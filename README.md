# Uciniti Vulkan Game Engine

## About
Uciniti (Učiniti - meaning render/make/fulfill) is a simple, in development, interactive application and rendering engine. The engine is in early stages of development and plenty more is to come.

## Building Uciniti
Uciniti is built using Visual Studio 2019, using x64 architecture. I would recommened building with the same target as the engine is untest in other envionrments. The engine is also currently on built for Windows systems.

Using your favourite git tool, simply start bt cloning the repository with `git clone --recursive https://github.com/EricB326/UcinitiVK2.git`
If you require cloning just the submodules, use the following `git submodule update --init`

## Short Term Goals
*Note: Uciniti is constantly evolving, because of this the most up-to-date and reliable location to see what is being worked on is over on the road map location at: *

As 2020 comes to a close, I am to have Uciniti in a state of being able to render any mesh passed to the engine using Vulkan as the rendering API. This includes rendering with PBR in mind.
Come late Decemeber the goal is to have this workflow hooked into ImGUI API to allow this mesh submission to occur on the front end of the engine.

## Long Term Goals
The big picture for Uciniti is to create a powerful 3D engine that I am proud to showcase on my portfolio. I want to focus on the rendering aspects and include fundamental graphic theory such as:

- Physically based rendering
- High dynamic range
- Shadows including multiple methods such as point shadows and shadow mapping
- Bloom
- And much more...

Along with these graphical theories I am to make the engine platform agnostic, meaning the implementation of multiple rendering APIs on top of the already implemented Vulkan. This includes:

- Metal
- OpenGL/DirectX 11 for the older GPUs
- DirectX 12

Although the engines primary rendering API is Vulkan, I will work to adapt and implement the always evolving technologies.

## Features
Rendering a triangle in Vulkan :)

## Libraries
These libraries are currently implemented.
* spglog - A fast C++ logging library used for console outputs.
* GLFW - A simple API for creating windows, contexts, and surfaces.
* GLM - A header only mathematics library for graphics software.
* Vulkan - A low-overhead, cross-platform 3D graphics and computing API.
* ImGUI - A graphical user inface library.
* Shaderc - A collection of tools, libraries, and tests for shader compilation.
* Glslang - A reference compiler front end for the GLSL shading languages.
* SPIR-V Tools - A tool that provides an API and commands for processing SPIR-V modules.
* stb_image - A image loading/decoding from file/memory header.

