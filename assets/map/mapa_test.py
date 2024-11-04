# coding=utf-8
"""Generating a 3D terrain map inspired by 1080° Snowboarding"""

import glfw
from OpenGL.GL import *
import OpenGL.GL.shaders
import numpy as np
import sys
import os.path
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import grafica.transformations as tr
import grafica.basic_shapes as bs
import grafica.easy_shaders as es
import grafica.performance_monitor as pm

__author__ = "Daniel Calderon"
__license__ = "MIT"

class Controller:
    def __init__(self):
        self.fillPolygon = False

controller = Controller()

def on_key(window, key, scancode, action, mods):
    if action != glfw.PRESS:
        return
    
    global controller

    if key == glfw.KEY_SPACE:
        controller.fillPolygon = not controller.fillPolygon
    elif key == glfw.KEY_ESCAPE:
        glfw.set_window_should_close(window, True)

def snowboarding_terrain(x, y):
    # Generating terrain using a combination of sine and cosine functions
    z = 0.1 * np.sin(0.5 * x) * np.cos(0.5 * y) + 0.1 * np.sin(2 * np.sqrt(x**2 + y**2))
    return z

def generateMesh(xs, ys, function, color):
    vertices = []
    indices = []
    data_to_save = []

    for i in range(len(xs)):
        for j in range(len(ys)):
            x = xs[i]
            y = ys[j]
            z = function(x, y)
            
            vertices += [x, y, z] + color
            data_to_save.append([x, y, z])  # Save (x, y, z) in the list

    index = lambda i, j: i * len(ys) + j 
    
    for i in range(len(xs)-1):
        for j in range(len(ys)-1):
            isw = index(i, j)
            ise = index(i + 1, j)
            ine = index(i + 1, j + 1)
            inw = index(i, j + 1)

            indices += [isw, ise, ine, ine, inw, isw]

    np.savetxt("map.csv", data_to_save, delimiter=",", header="x,y,z", comments="")
    return bs.Shape(vertices, indices)

if __name__ == "__main__":
    if not glfw.init():
        glfw.set_window_should_close(window, True)

    width = 1000
    height = 1000
    title = "Snowboarding Terrain Map"
    window = glfw.create_window(width, height, title, None, None)

    if not window:
        glfw.terminate()
        glfw.set_window_should_close(window, True)

    glfw.make_context_current(window)
    glfw.set_key_callback(window, on_key)

    pipeline = es.SimpleModelViewProjectionShaderProgram()
    glUseProgram(pipeline.shaderProgram)
    glClearColor(0.85, 0.85, 0.85, 1.0)
    glEnable(GL_DEPTH_TEST)

    cpuAxis = bs.createAxis(7)
    gpuAxis = es.GPUShape().initBuffers()
    pipeline.setupVAO(gpuAxis)
    gpuAxis.fillBuffers(cpuAxis.vertices, cpuAxis.indices, GL_STATIC_DRAW)

    # Create terrain function
    simpleTerrain = lambda x, y: snowboarding_terrain(x, y)

    xs = np.ogrid[-10:10:40j]
    ys = np.ogrid[-10:10:40j]
    cpuSurface = generateMesh(xs, ys, simpleTerrain, [0.5, 0.5, 1.0])
    gpuSurface = es.GPUShape().initBuffers()
    pipeline.setupVAO(gpuSurface)
    gpuSurface.fillBuffers(cpuSurface.vertices, cpuSurface.indices, GL_STATIC_DRAW)

    t0 = glfw.get_time()
    camera_theta = np.pi / 7
    perfMonitor = pm.PerformanceMonitor(glfw.get_time(), 0.5)
    glfw.swap_interval(0)

    while not glfw.window_should_close(window):
        perfMonitor.update(glfw.get_time())
        glfw.set_window_title(window, title + str(perfMonitor))
        glfw.poll_events()

        t1 = glfw.get_time()
        dt = t1 - t0
        t0 = t1

        if (glfw.get_key(window, glfw.KEY_LEFT) == glfw.PRESS):
            camera_theta -= 2 * dt
        if (glfw.get_key(window, glfw.KEY_RIGHT) == glfw.PRESS):
            camera_theta += 2 * dt

        camX = 10 * np.sin(camera_theta)
        camY = 10 * np.cos(camera_theta)

        viewPos = np.array([camX, camY, 10])
        view = tr.lookAt(viewPos, np.array([0, 0, 0]), np.array([0, 0, 1]))

        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "view"), 1, GL_TRUE, view)
        projection = tr.perspective(100, float(width) / float(height), 0.1, 100)
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "projection"), 1, GL_TRUE, projection)

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "model"), 1, GL_TRUE, tr.identity())
        pipeline.drawCall(gpuAxis, GL_LINES)

        if (controller.fillPolygon):
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
        else:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)

        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "model"), 1, GL_TRUE, tr.uniformScale(0.5))
        pipeline.drawCall(gpuSurface)

        glfw.swap_buffers(window)

    gpuSurface.clear()
    gpuAxis.clear()
    glfw.terminate()
