# OpenVR Module

## Requirements
- OpenVR Plugin in Render Pipeline



## Usage
Enable `openvr` module and follow the usage in
[OpenVR Plugin](https://github.com/bluekyu/rpcpp_plugins/tree/master/openvr)

### Summary
Simply, copy `config/panda3d` directory in `example/1-Graphics/08-OpenVR` example and
add `openvr` module to `config/SystemConfiguration.xml` file.

Or, edit configuration files according to following:

1. `config/SystemConfiguration.xml`: Add `openvr` module.
2. `config/panda3d/render_pipeline/plugins.yaml`: Enable `openvr` plugin.
3. Enable stereo mode (`pipeline.yaml`, `stage.yaml`)
   - `pipeline.yaml`: Use `left`, `right`, `stereo` value in `stereo_mode` line.
   - `stage.yaml`: Add `OpenVRRenderStage` before `UpscaleStage`
4. Controlling Rendering Resolution
   - Window Size (`Config.prc`)
     - Use HMD resolution `width x height` to `win-size`
   - OR Fixed Window Size (`pipeline.yaml`)
     - `resolution_scale`: 0.0
     - Use HMD resolution `width x height` to `resolution_width` and `resolution_height`.
     - Cropping: Set `screen_cropping` to `true`

### Documents
- [Render Pipeline OpenVR](https://github.com/bluekyu/rpcpp_plugins/blob/master/openvr/docs/openvr.md)
- [Stereo and Virtual Reality in Render Pipeline](https://github.com/bluekyu/render_pipeline_cpp/blob/master/docs/ko_kr/rendering/stereo-and-vr.md)
