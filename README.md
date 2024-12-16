# OpenGLRobotShootingGame
This is a simple first-person robot shooting game coded with OpenGL. This is a round-based game where round information is seen in the console with working projectile collisions, detections, and robot animations. 

# Blitzcrank 3D Shooting Game
# by: Matthew Evangelista
# email: matthew.evangelista@torontomu.ca
===========================================

Included in this Zip Are:
1. CPS511Blitzcrank Solution Folder
2. This README file

Features:
1. Round-Based Shooting Game with Non-Texture Mapped Hierarchical Bots walking forward shooting at 
the player/cannon
2. Cannon that shoots projectiles (as of the moment, the cannon does not sync with camera BUT
animation is played when player hits 0 hp)
3. Collision Detection with Bounding Box Struct
4. Increase of Blitzcrank Bot amount, player health, and player projectile upon round completion (up to 15 rounds where win is achieved)

Missing Features:
1. Vertex/Fragment Shader
2. Custom Mesh
3. Cannon DOES NOT move with camera

Bonus:
1. Rounds, but no score. Each round increases player projectile speed, blitzcrank amount, and 3
player health.

How To Compile (Visual Studio 2022):
===========================================

1. Drag the "CPS511Blitzcrank" to any folder (Desktop, etc.) and take note of where it is.
2. Open Visual Studio 2022, and on the homepage select "Open Project"
3. When File Explorer opens, navigate to the CPS511Blitzcrank folder, and select CPS511Blitzcrank.sln
4. If it opens the solution/project in Visual Studio 2022, press Ctrl+Shift+B or "Build Solution"
5. Run the program with CTRL+F5 or the green play button at the top.

Controls:

1. Mouse: Aim Camera/"Cannon"
2. Spacebar: Shoot
3. Lowercase 'r': Reset Game
