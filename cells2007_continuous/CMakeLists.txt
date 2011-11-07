cmake_minimum_required(VERSION 2.6)

project(cells2007_continuous)

find_package(wxWidgets COMPONENTS core base REQUIRED)
# we need version 2.9 or higher but http://public.kitware.com/Bug/view.php?id=10694
include( "${wxWidgets_USE_FILE}" )

add_executable(cells2007_continuous
  WIN32
  MACOSX_BUNDLE
  C2dVector.cpp
  C2dVector.h
  SquirmCell.cpp
  SquirmCell.h
  SquirmCellProperties.cpp
  SquirmCellProperties.h
  SquirmCellSlot.cpp
  SquirmCellSlot.h
  SquirmChemistry.cpp
  SquirmChemistry.h
  SquirmError.cpp
  SquirmError.h
  SquirmGrid.cpp
  SquirmGrid.h
  SquirmGrid_wx.cpp
  SquirmGrid_wx.h
  SquirmReaction.cpp
  SquirmReaction.h
  window.cpp
)

target_link_libraries(cells2007_continuous ${wxWidgets_LIBRARIES} )


