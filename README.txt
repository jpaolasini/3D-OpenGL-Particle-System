The Following program was developed in OSX Mavericks
using Xcode. 

Additional features that were added include


-lighting
-particle shape changes (press c)
-bombs (press b)
-stage elasticity(d and e)

-turn stage into water (press w)

**Bombs that fall into water explode instantly.



/*Important*** Particle lifespan is not time based,In order to keep a fluent animation
 the lifespan of a particle is based on its Y value or Bounce count. Bomb fragments are 
 based on bounce count.
 */

Beware of the Bomb By Jason Paolasini
-------------------------------------------
up= Camera Pan +Z
down= Camera Pan -Z
left= Camera Pan -X
right= Camera Pan +X
F1= Camera Pan +Y
F2= Camera Pan -Y
+= increase fountain flow
-= decrease fountain flow
SPACE = pause simulation
r= reset animation
Q or Esc= QuitProgram
-------------------------------------------
THE BOMBS = b (Special Feature 1)
-------------------------------------------

When the b button is pressed, a bomb will be generated


-------------------------------------------
WATER = w (Special Feature 2)
-------------------------------------------
When w is pressed the stage will toggled into a water-like substance.

-------------------------------------------
Toggle Shapes = c (Special Feature 3)
-------------------------------------------
In order to see the rotations of all shapes, squares stop once 
they have landed, you can toggle the shapes to 
Cubes, press c to toggle
-------------------------------------------
e= increase elasticity of the stage
d= reduce elasticity of the stage
f= toggle friction(initially on)

end