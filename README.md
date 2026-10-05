# Mapping Third-Party

ROS mapping dependencies used by the ADMM-CLM workspace. This repository is a
vendored collection of upstream projects with local integration changes, not a
single original mapping library.

Public repository: [GTLIDAR/mapping_third_party](https://github.com/GTLIDAR/mapping_third_party).

## Components

| Directory | Purpose | Original upstream |
| --- | --- | --- |
| `elevation_mapping` | C++ elevation mapping | [ANYbotics/elevation_mapping](https://github.com/ANYbotics/elevation_mapping) |
| `elevation_mapping_cupy` | GPU mapping and C++ plane-segmentation packages | [leggedrobotics/elevation_mapping_cupy](https://github.com/leggedrobotics/elevation_mapping_cupy) |
| `grid_map` | Grid-map storage, processing, ROS interfaces, and RViz visualization | [ANYbotics/grid_map](https://github.com/ANYbotics/grid_map) |
| `kindr` | Kinematics and rotation representations | [ANYbotics/kindr](https://github.com/ANYbotics/kindr) |
| `kindr_ros` | ROS conversions, messages, and visualization for kindr | [ANYbotics/kindr_ros](https://github.com/ANYbotics/kindr_ros) |
| `message_logger` | Logging utilities | [ANYbotics/message_logger](https://github.com/ANYbotics/message_logger) |
| `point_cloud_io` | Point-cloud file input/output | [ANYbotics/point_cloud_io](https://github.com/ANYbotics/point_cloud_io) |

The collection was copied from
[DRCL-USC/Quadruped_Wrapper's third_party directory](https://github.com/DRCL-USC/Quadruped_Wrapper/tree/837f51807e192241437c642bc5dc9c93a46224f0/third_party)
at commit `837f51807e192241437c642bc5dc9c93a46224f0`. Local directories may
differ from their upstream versions. Integration changes include RViz grid-line
rendering updates, mapping numerical safeguards, and removal of the obsolete
RealSense Gazebo plugin.

## Build and Use

Use a ROS Noetic catkin workspace with `catkin_tools`. From the workspace root:

```bash
source /opt/ros/noetic/setup.bash
catkin config --extend /opt/ros/noetic --cmake-args -DCMAKE_BUILD_TYPE=Release
catkin build elevation_mapping convex_plane_decomposition_ros point_cloud_io \
  grid_map_rviz_plugin --jobs 4 --parallel-packages 1
source devel/setup.bash
```

Install the required ROS and system dependencies before building; see each
component's package manifest and README. The `cgal5_catkin` wrapper downloads
CGAL 5.3 during the initial build, requiring network access. Reduce `--jobs`
if memory is limited.

ADMM-CLM uses the C++ elevation mapper and the CPU plane-decomposition pipeline,
not the CuPy GPU mapper. CUDA/CuPy are not required for this CPU pipeline.
GPU mapping and upstream demos have additional requirements described in the
[elevation_mapping_cupy README](elevation_mapping_cupy/README.md).
See the [plane-segmentation README](elevation_mapping_cupy/plane_segmentation/README.md)
for standalone launch commands.

When used inside the ADMM-CLM workspace, its shared mapping launch is:

```bash
roslaunch ocs2_multi_robot elevation_mapping_fixed.launch \
  world_name:=gap_slope_course
```

This launch belongs to `ocs2_multi_robot`, not this repository, and requires the
workspace's terrain assets and configured TF frames.

## License and Citation

There is no single license covering the entire collection. The ANYbotics
components retain their BSD-3-Clause licenses; `elevation_mapping_cupy` retains
its MIT license. Embedded and fetched dependencies have their own terms,
including EigenLab and CGAL. Preserve component license files and source-file
copyright notices; the collection is not MIT-only.

The CPU plane-decomposition code uses CGAL 5.3 Shape_detection
(GPL-3.0-or-later) and Polygon (LGPL-3.0-or-later). Review the applicable
combined-binary and corresponding-source obligations before distributing
executables or Docker images; merely including license texts does not establish
complete compliance.

Follow the citation instructions in the
[grid_map](grid_map/README.md#publications),
[elevation_mapping](elevation_mapping/README.md#citing), and
[elevation_mapping_cupy](elevation_mapping_cupy/README.md) READMEs for the
methods actually used. Credit their original authors, not only this
collection's maintainer.
