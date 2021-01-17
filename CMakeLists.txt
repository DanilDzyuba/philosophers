cmake_minimum_required(VERSION 3.17)
project(philosophers C)

set(CMAKE_C_STANDARD 99)

add_executable(philosophers philo_two/main.c philo_one/utils.c philo_two/philo_two.h)