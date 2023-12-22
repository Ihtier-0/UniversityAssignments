cmake_minimum_required(VERSION 3.5)

project(numerical_analysis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(application
  application.cpp
  application.h)
source_group("application" FILES ${application})

set(bisection_method
  bisection_method.h
  bisection_method_test.cpp
  bisection_method_test.h)
source_group("bisection_method" FILES ${bisection_method})

set(gradient
  symmetric_derivative_gradient.h)
source_group("gradient" FILES ${gradient})

set(gradient_descent
  gradient_descent.h
  gradient_descent_test.cpp
  gradient_descent_test.h)
source_group("gradient_descent" FILES ${gradient_descent})

set(gui
  main_window.cpp
  main_window.h)
source_group("gui" FILES ${gui})

set(utils
  out_utils.h)
source_group("utils" FILES ${utils})

set(qcustomplot
  qcustomplot.cpp
  qcustomplot.h)
source_group("qcustomplot" FILES ${qcustomplot})

set(runge_kutta
  runge_kutta.h
  runge_kutta_test.cpp
  runge_kutta_test.h)
source_group("runge_kutta" FILES ${runge_kutta})

set(types
  types.h)
source_group("types" FILES ${types})

add_executable(${CMAKE_PROJECT_NAME}
  ${application}
  ${bisection_method}
  ${gradient}
  ${gradient_descent}
  ${gui}
  ${utils}
  ${qcustomplot}
  ${runge_kutta}
  ${types}
  main.cpp
)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY AUTOMOC ON)

find_package(Qt5 REQUIRED
  COMPONENTS Core Gui Widgets PrintSupport)
target_link_libraries(${CMAKE_PROJECT_NAME}
  PRIVATE
  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)

install(
  TARGETS
  ${CMAKE_PROJECT_NAME}
  LIBRARY
  DESTINATION
  ${CMAKE_INSTALL_LIBDIR})
