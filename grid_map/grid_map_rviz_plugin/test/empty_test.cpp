#include <gtest/gtest.h>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <grid_map_ros/GridMapRosConverter.hpp>
#include <grid_map_rviz_plugin/GridMapVisual.hpp>
#include <rviz/ogre_helpers/render_system.h>

TEST(GridMapVisual, GridLinesWithoutColorStayWithinCapacity) {
  rviz::RenderSystem::get();
  auto* root = Ogre::Root::getSingletonPtr();
  auto* sceneManager = root->createSceneManager(Ogre::ST_GENERIC);
  {
    grid_map_rviz_plugin::GridMapVisual visual(sceneManager, sceneManager->getRootSceneNode());
    for (const int cellCount : {2, 120, 121}) {
      grid_map::GridMap map({"elevation"});
      map.setFrameId("odom");
      map.setGeometry(grid_map::Length(cellCount * 0.05, cellCount * 0.05), 0.05);
      map["elevation"].setZero();
      grid_map_msgs::GridMap::Ptr message(new grid_map_msgs::GridMap);
      grid_map::GridMapRosConverter::toMessage(map, *message);
      visual.setMessage(message);
      for (const int decimation : {1, 2, 7}) {
        for (const bool noColor : {true, false}) {
          SCOPED_TRACE(::testing::Message() << "cells=" << cellCount << " decimation=" << decimation
                                           << " noColor=" << noColor);
          EXPECT_NO_THROW(visual.computeVisualization(
              0.5, true, false, "elevation", false, noColor, Ogre::ColourValue::White,
              false, "elevation", "default", true, false, Ogre::ColourValue::Black,
              Ogre::ColourValue::White, false, 0.0, 0.15, 0.05, decimation));
        }
      }
    }
  }
  root->destroySceneManager(sceneManager);
}
