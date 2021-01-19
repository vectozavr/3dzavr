# 3dzavr
Simple 3d engine based on SFML library.
I try to make this engine powerful and easy to understand.

The main goal - to implement basic 3D graphics from scratch (without OpenGL or any other 3D graphics library)
For this moment I already implemented%
- Operations with Point4D, Matrix4x4, Triangles and Mesh. I used operators overloading to simplify coding.
- Projections from 3D on 2D plane, movement and rotation of camera and meshes.
- Triangles clipping
- Basic smooth animations for meshes
- External camera (You can see how 3D world projected on 2D plane and the procces pf clipping)

Future TODO:
- Animation of camera
- Improvement of animations by using Bézier curves
- Textures

Demonstration of how does this engine work:
![Project demonstration](img/external_camera_1.png)
![Project demonstration](img/clipping.png)
![Project demonstration](img/mountains.png)
![Project demonstration](img/external_camera_2.png)
![Project demonstration](img/teapot.png)
![Project demonstration](img/girl.png)
