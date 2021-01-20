cmake_minimum_required(VERSION 3.17)
project(philosophers C)

set(CMAKE_C_STANDARD 99)

add_executable(philosophers philo_three/main.c philo_three/philo_three.h philo_three/utils.c)