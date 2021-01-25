# 3dzavr
Simple 3d engine based on SFML library.
I tried to make this engine powerful and easy to understand.

The main goal - to implement basic 3D graphics from scratch (without OpenGL or any other 3D graphics library)

For this moment I already implemented:
- Operations with Point4D, Matrix4x4, Triangles and Mesh. I used operators overloading to simplify coding.
- Projections from 3D on 2D plane, mcovement and rotation of camera and meshes.
- Triangles clipping
- Advanced smooth animations by using Bézier curves for meshes and camera
- External camera (You can see how 3D world projected on 2D plane and the procces pf clipping)
- You can render animated scene to .mp4 files or sequence of .png files (film/img/ & film/mp4). But it is extremely slow process due-to SFML waste a lot of time on saving .png files. It is still better to capture your screen by some additional software.

TODO:
- Physics engine
- Ray-tracing & recursive ray-tracing 
- Textures

Demonstration of how does this engine work:
![hippo](gif/teapot.gif)
![hippo](gif/cube.gif)
![Project demonstration](img/external_camera_1.png)
![Project demonstration](img/clipping.png)
![Project demonstration](img/mountains.png)
![Project demonstration](img/external_camera_2.png)
![Project demonstration](img/teapot.png)
![Project demonstration](img/girl.png)
