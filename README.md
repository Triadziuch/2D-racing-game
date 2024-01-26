# Ścigałka
2D top down racing game in C++ using SFML as graphicd library and custom game engine using STM32 with IKS01A3 board for movement

![2024-01-2614-56-17_3-ezgif com-optimize (1)](https://github.com/Triadziuch/2D-racing-game/assets/75269577/62b50f53-b02f-4681-95a5-03b2a7b77fee)

## Project Description
"Ścigałka" is a project created for Computer Architecture classes. The objective of this project was to transmit data between the STM32L476RG microcontroller and a program running on a computer using the COM port. To demonstrate that the data can be utilized in real-time, I decided to create a simple game that receives information about the rotation of the STM32 microcontroller and translates it into the rotation of the car controlled by the player.

## Gameplay
During the gameplay, the player controls a car moving along the road and must dodge various obstacles. These obstacles include other cars that explode upon collision, cracks that reduce steering control and speed, oil spills and ice patches that cause skidding, and potholes that reset the player's speed. To increase speed and the points multiplier, the player can also drive over speed boost pickups.

After covering a specified distance, there is a smooth transition to different maps. There are four distinct maps:
* Forest
* Desert
* Bridge
* Arctic

## Running the Game:
To run the game, download the appropriate release and execute the .exe file. By default, the game is controlled using the mouse, but there is also an option to run the game controlled by a microcontroller, provided the appropriate program is uploaded [revelant STM32L476RG rotation transmitting repository](). To enable the game to use the microcontroller, change the first line of the config.txt file to "on".

## Special Thanks
Special thanks to [GrafxKid](https://grafxkid.carrd.co/), whose graphics were utilized in the project.
