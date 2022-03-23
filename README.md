## Cook-Bot

This project consists of 4 Robots which can collect ingredients from 3 dispensers and drop it in the cooking station.

All Robots follow a fixed path which starts from the rest position of the robot to the Dispenser, then to the cooking station and back to the rest position itself

The CSV file consists of the path nodes along with weights to help decide the path itself. The planner used was a simple bug algorithm.

The Map file consists of the map of the robot. A Semaphore server helps to avoid locking of two or robot, which could prevent the motion of the robot itself.

Ever robot has a client running which also gives the battery level of each robot. Once the robot battery's drops to a certain threshold, it automatically moves to the docking site for charging.

Depending on the number of food orders, the robots' trajectories change.

```typescript
In order to run this code run the arm_fleet.demo10.launch file
```
which will launch the server and the robot launch files.

For more information about the project, send me a mail @ parthanvelanjeri@hotmail.com

Credits to my friend Gokulraj for helping with this project !
