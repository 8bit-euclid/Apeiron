**General**
- [x] GitHub actions / CI
- [ ] Pre-compiled headers
- [ ] C++ Modules integration
- [ ] Stack frame write-out
- [ ] Installation/setup (Readme file)
- [ ] Dependency list (Readme file)
- [ ] Use chronos duration for time units
- [ ] Use pure virtual interfaces where possible

**New Features**
- [x] Off-screen rendering
- [x] HDR
- [x] Bloom
- [x] Update Texture interface to offer more versatility
- [x] Multi-sampled frame buffer attachments
- [x] Update shadow shaders to account for transparency
- [x] ImGui re-integration
- [ ] Add GUI switches for post-processing effects
- [ ] LaTeX text/math rendering
- [ ] Deprecate String
- [ ] Background texture embedding onto text
- [ ] Texture coordinate transformation uniforms in shaders
- [ ] Resurrect line shader
- [ ] Simple visualisation of lights and cameras
- [ ] Toggles/sliders for all visualiser settings
- [ ] RenderDoc debugger
- [ ] Perlin noise generator
- [ ] Curl noise generator
- [ ] Implement MSAA frame buffers for directional/point shadow maps
- [ ] Motion blur
- [ ] Deferred rendering
- [ ] Screen-space ambient occlusion (SSAO)
- [ ] PBR
- [ ] GDB debugger
- [ ] Model loading using Assimp
- [ ] Depth of field
- [ ] Vulkan integration
- [ ] Ray-tracing
- [ ] Gnuplot writer
- [ ] Pyplot writer
- [ ] Wireframe vs solid models
- [ ] Resurrect spotlights

**Bugs**
- [x] Check for possible double Gamma correction between HDR and blur shaders
- [x] Investigate Texture move constructor/assignment bug
- [x] Fix border shrinking bug during off-screen rendering
- [ ] Investigate possible bug causing asymmetry in point light sources
- [ ] Fix parallax occlusion bug

**Minor**
- [ ] Fix segmentation fault when exiting glfw context
