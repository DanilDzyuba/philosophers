cmake_minimum_required(VERSION 3.17)
project(philosophers C)

set(CMAKE_C_STANDARD 99)

add_executable(philosophers philo_one/main.c philo_one/philo_one.h philo_one/utils.c philo_one/dinner_helper.c)