/*Beware of the Bomb by Jason Paolasini
 Student Number 1162915

 */


#include <stdlib.h>
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include "3DMathLib.h"
#include <math.h>
#include <iostream>
#include <vector>
#include "Particle.h"
#include <ctime>
#include <time.h>


/*Important*** Particle lifespan is not time based,In order to keep a fluent animation
 the lifespan of a particle is based on its Y value or Bounce count. Bomb fragments are 
 based on bounce count.
 */



//Global Variables


float camPos[] = {-115, 30, 150};
float angle = 0.0f;
int numberOfParticles = 20;
float gravity = 9.8;
bool isWater = false;
bool aBomb = false;
int ballRate =5000;
bool onPlatform = true;
int particleCounter = 0;
float elasticity = 0.6;
bool friction = true;
bool circles = true;
bool paused = false;





//Lighting Values

float light_pos[] = {20.0, 20.0, 150.0, 1.0};

float amb0[4]  = {1, 1, 1, 1};
float diff0[4] = {1, 1, 1, 1};
float spec0[4] = {1, 1, 1, 1};


/*Using Lighting, it was important to store the materials that we would be using in a type of datastore
 The Struct was used to store all material properties*/



typedef struct materialStruct {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emission[4];
    float shininess;
} materialStruct;

materialStruct redPlastic = {
    {0.3, 0.0, 0.0, 1.0},
    {0.6, 0.0, 0.0, 1.0},
    {0.8, 0.6, 0.6, 1.0},
    {0.0, 0.0, 0.0, 0.0},
    32.0};

materialStruct water = {
    {0.0,0.0,0.3},
    {0.0,0.0,0.6},
    {0.6,0.6,0.8},
    {0.0,0.0,0.0,0.0},
    100.0};

materialStruct blackPlastic = {
    {0.03,0.03,0.03},
    {0.4,0.4,0.4},
    {0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0},
    100.0};

materialStruct whitePlastic = {
    {0.3,.3,0.3},
    {0.9,0.9,0.9},
    {0.1,0.7,0.7},
    {0.0,0.0,0.0,0.0},
    100.0};
materialStruct yellowPlastic = {
    {1,1,0},
    {0.9,0.9,0.9},
    {0.1,0.7,0.7},
    {0.0,0.0,0.0,0.0},
    100.0};

materialStruct gold = {
    {0.33, 0.22, 0.03, 1.0},
    {0.78, 0.57, 0.11, 1.0},
    {0.0, 0.0, 0.0, 0.0},
    {0.99, 0.91, 0.81, 1.0},
    100.8};







/*Generate the vector that stores all the particles. A vector was used because it is a dynamic datastructure that
can be modified as we add particles*/



std::vector<particle> particles;




vec3D up (0,0.01,0);

point3D origin(0,0.00000000000001,0);




void notesToTheTA(void){
    
    printf("Beware of the Bomb By Jason Paolasini\n-------------------------------------------\nup= Camera Pan +Z\ndown= Camera Pan -Z\nleft= Camera Pan -X\nright= Camera Pan +X\nF1= Camera Pan +Y\nF2= Camera Pan -Y\n+= increase fountain flow\n-= decrease fountain flow\nSPACE = pause simulation\nr= reset animation\nQ or Esc= QuitProgram\n-------------------------------------------\nTHE BOMBS = b (Special Feature 1)\n-------------------------------------------\n\nWhen the b button is pressed, a bomb will be generated\n\n\n-------------------------------------------\nWATER = w (Special Feature 2)\n-------------------------------------------\nWhen w is pressed the stage will toggled into a waterlike substance.\n\n-------------------------------------------\nToggle Shapes = c (Special Feature 3)\n-------------------------------------------\nIn order to see the rotations of all shapes, squares stop once \nthey have landed, you can toggle the shapes to \nCubes, press c to toggle\n-------------------------------------------\ne= increase elasticity of the stage\nd= reduce elasticity of the stage\nf= toggle friction(initially on)");
}







//The size of the particles range from 1-2

float randomSize(){
    float size = 1+(rand()%10)/10;
    return size;
}





vec3D directionUp(){
    
    float x = -0.5+1*((rand()%360)/360.0);
    float y = 1+2*((rand()%100)/100.0);
    float z = -0.5+1*((rand()%360)/360.0);
    
    
    vec3D dirVec(x,y,z);
    
    return dirVec;
}





//Random color is used when lighting is turned off.

colour randomColour(void){
    
    colour col1(0.1+1/(0.1+1+rand()%7),1/(0.1+1+rand()%7), 1/(1+rand()%5));
    

        return col1;
    
  
}






double randomSpeed(void){
    double speed = 1/(1+rand()%10);
    
    return speed;
}





//Used to easily index through the particle list and remove it.

void clearParticle(particle particle1,int i){
    
        
            particles.erase(particles.begin()+i);
            
    
}





vec3D randomSpin(){
    vec3D newSpin(10*(rand()%10/10.0),rand()%10/10.0,10*(rand()%10/10.0));
    
    return newSpin;
    
    
}





/*Generates particles with random initial variables speed, color, size, spin and sets their origin, and type
 When a particle is created in this system, it can be either a bomb or not.The bombs have special attributes
 when it comes to material and shape, this is taken care of in the display() function.*/

void newParticles(point3D origin){
    
    
    
    if(aBomb){
        
        particle particle1(origin,randomSpeed(),directionUp(),randomSpin(),3,randomColour(),0,true);
        particles.push_back(particle1); //pushes point onto the vector
        
        aBomb =false;
    }
    
    
    else
    {
        particle particle1(origin,randomSpeed(),directionUp(),randomSpin(),randomSize(),randomColour(),0,false);
        particles.push_back(particle1); //pushes point onto the vector
    }
    
}








//Posts Redisplay on a timed interval, in our case we have chosen 5ms to render a smooth animation

void timer(int id){
    glutPostRedisplay();
}







/*Because the display refreshes every 5ms the movement of the particle
 is determined by the equation velocity = velocity-acceleration*time
*/


vec3D rise(particle pt1){
    
    pt1.direction.y=pt1.direction.y-gravity*(0.005) ;
    return pt1.direction;
    
}







/*Responsible for the movement of the particles, the function controlls the velocity before and after contact with the 
 Platform */

void moveParticles(void){
    
    for (int i = 0; i<particles.size(); i++) {
        
        
        //Checks to see if the particle is still on the platform
        
        
        if((particles[i].location.z<80)&&(particles[i].location.z>-80) && (particles[i].location.x<80)&&(particles[i].location.x>-80)){
            
            onPlatform =true;
        }
        
        
        else{
            onPlatform =false;
        }
        
        //Detects the bounce of a particle based on the y location.
      
        if (particles[i].location.y<=0 && onPlatform) {
            particles[i].location.y = 0.0;
            particles[i].direction.y = particles[i].direction.y*-1;
            particles[i].direction.y =elasticity*(particles[i].direction.y);
            particles[i].bounce=particles[i].bounce+1;
            
        }
        
        
        
        /*Used as a way to age the particles. Any particle that falls off the platform
         will be deleted to save space in the computers memory and not continue to grow the
         particle vector
        */
        
        else if(particles[i].location.y<-200){
            clearParticle(particles[i], i);
        }
      
        
            particles[i].direction = rise(particles[i]);
            particles[i].location = particles[i].location.shift(particles[i].direction);
       
        
    }
}



//resets the particles and the camera position

void reset(){
    for(int i = 0; i<particles.size();i++){
   
        clearParticle(particles[i], i);
       
        
    }
    
    //reinitialize global variables
    
    camPos[0] =-115;
    camPos[1] = 30;
    camPos[2] =150;
    isWater = false;
    ballRate =5000;
    particleCounter = 0;
    elasticity = 0.6;
    friction = true;
    paused = false;
    circles =true;

}


//All keyboard controls


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'q':
		case 27:
			exit (0);
			break;
        case 'a':
            newParticles(origin);
            break;
        case 'e':
            elasticity+=0.05;
            break;
            
        case 'd':
            elasticity-=0.05;
            break;
            
        case 'w':
            if(!isWater){
            elasticity = -.90000;
                isWater = true;}
            else{
                isWater = false;
                elasticity = 0.5;}
            break;
            
        case 'f':
            if (friction){
                friction = false;
            elasticity = 1.0;
            }
            else{
                friction = true;
                elasticity = 0.6;
            }
            break;
            
        case 'b':
            aBomb=true;
            break;
            
        case 'c':
            
            if(!circles){
                circles=true;
            }
            else{
                circles=false;
            }
            break;
            
        case '+':
            
            if(ballRate>1000){
                
            ballRate-=1000;
            }
            
            break;
 
        case '-':

            ballRate+=1000;
            break;
            
        case 'r':   //ensures all particles are deleted
            while(particles.size()>0){
            reset();
            }
            break;
            
            
        case 32:
            if(paused){
                paused = false;
            }
            else{
                paused = true;
            }
            
        default:
            break;
	}
}









void init(void)
{
	glClearColor(0.25, 0.25, 0.25, 0); //sets backgorund to grey

    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 500);
 
    
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
   
    
}




// In-order to quickly call material changes, The glMaterial calls are quickly accessed by key value

void setMaterial(char key){
    
    switch (key) {
        case 'r':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,redPlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, redPlastic.shininess);
            
            break;
            
        case 'b':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,water.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, water.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, water.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, water.shininess);
            
            break;
            
        case 'B':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,blackPlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackPlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackPlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, blackPlastic.shininess);
            
            break;
            
        case 'w':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,whitePlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whitePlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whitePlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, whitePlastic.shininess);
            
            break;
        
        case 'y':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,yellowPlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellowPlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellowPlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,yellowPlastic.shininess);
            
            break;
            
            case 'g':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,gold.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,gold.shininess);
            break;
        
        default:
            break;
    }
    
    
    
    
}




/*instead of controlling the material changes in the display function, it was modularized into its own function.
 This function, alternates bomb colours on bounce, and sets all other
 */



void setParticleMaterial(particle particleIn){
    
    
    if(particleIn.bomb && (particleIn.bounce%2==0)){
        
        setMaterial('B');
        
    }
    
    //Quality of all bomb fragments are over 5 and still a bomb, these are black
    
    else if(particleIn.bomb && (particleIn.bounce>=5)){
       
        setMaterial('B');
    }
    
    
    else if(particleIn.bomb && (particleIn.bounce%2==1)){
        
        setMaterial('r');
        
    }

    else{
        
        setMaterial('g');
    }
    
}







//Generates 1200 small particles at the location of the explotion

void explode(particle theBomb){
    
    
        for (int i = 0; i<4200; i++) {
      
            //New direction of the fragment
            
            vec3D direction(2*(1-(rand()%200/100.0)),1*(rand()%300/100.0),2*((1-rand()%200/100.0)));
            
            
            //Creates Fragment
            
            particle particle1(theBomb.location,randomSpeed(),direction,randomSpin(),-0.5,randomColour(),5,true);
            
            
            //pushes particle onto the vector
            
            particles.push_back(particle1);
        

        
        }

    
}





void display(void)

    
{
    if(!paused){
    angle++;    //a general counter that is used to increment roll of particles
    }
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glFrustum(-800, 800,-800, 800,800, 800);
	gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    
    

    glutKeyboardFunc(keyboard);
    
    
    
    glBegin(GL_QUADS);
    
    glNormal3f(0.707, 0, 0.707); //set the normal for lighting

    
    //Checks the water condition and will set the stage blue or standard white
    
    if(!isWater){
        setMaterial('w');
    }
    
    else{
        setMaterial('b');
    }
    
    
    
    
    glVertex3f(80, -1.5, -80 );
    glVertex3f(80, -1.5, 80 );
    glVertex3f(-80,-1.5, 80 );
    glVertex3f(-80, -1.5, -80 );

    
    glEnd();
    

    
    
    if(!paused){
    
    moveParticles();    //call to move particles within the 5ms interval the appropriate amounts
    
    }
    
    
    for (int i = 0; i<particles.size(); i++) {
    
        
        
        glPushMatrix();
        
        
        //Translates the particle according to the changes made in moveParticles();
        
        glTranslatef(particles[i].location.x, particles[i].location.y, particles[i].location.z);
        

        
        /*Rotates particles based off their spin value, stops the rotation of objects once they are on the stage
         This was done because squares do not roll irl.
         */
        
        if(particles[i].location.y>=0.0){
            
            glRotatef(abs(angle*particles[i].spin.magnitude()), particles[i].spin.x,particles[i].spin.y,particles[i].spin.z);
        
        }
     
        
        
        
        setParticleMaterial(particles[i]);
        
        
        
        /*As the display function iterates through the particles in the system it checks for bouncing bombs and 
         bouncing bomb fragments, on the 4th bounce the explode() function is called, also after 12 bounces all fragments are destroyed to save space in the memory
        */
        
        
        if(particles[i].bomb &&( particles[i].bounce == 4)){
            
            explode(particles[i]);
            clearParticle(particles[i], i);
        
            
        }
        
        
        if(particles[i].bomb && particles[i].bounce>7){
          particles.erase(particles.begin()+i);
        }
       
        
        
        
        /*allows for the user to toggle between cubes and spheres based on their selection
         The bombs have always been sleected as cubes because the rendering of 1200 sphere 
         fragments is too processor intensive*/
        
        
        if(!circles||particles[i].bomb){
            
            glutSolidCube(particles[i].size+0.7);
            
        }
        
        
        else{
            
            glutSolidSphere(particles[i].size,20,20);
            
        }
        
        
    glPopMatrix();
   
    }

   
    
    //The timerFunc is set to redisplay every 5ms
    
	glutSwapBuffers();
    glutTimerFunc(5,timer,0);
    
    
}



void special(int key, int x, int y)
{
	//arrow key presses move the camera
	switch(key)
	{
		case GLUT_KEY_LEFT:
			camPos[0]-=2.5;
			break;
            
		case GLUT_KEY_RIGHT:
			camPos[0]+=2.5;
			break;
            
		case GLUT_KEY_UP:
			camPos[2] -= 2.5;
			break;
            
		case GLUT_KEY_DOWN:
			camPos[2] += 2.5;
			break;
            
		case GLUT_KEY_F1:
			camPos[1] += 2.5;
			break;
            
		case GLUT_KEY_F2:
			camPos[1] -= 2.5;
			break;
            
	}
	
}



//Continually generates particles at the users selected rate set by ballCount

void idle(){
    
        particleCounter++;
    
    
        if(particleCounter%ballRate==0 && !paused){
        
            newParticles(origin);
            
        }

    
}





int main(int argc, char** argv)
{
    
    srand(time(0));
    
    glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

    
	glutCreateWindow("Beware of the Bombs");	//creates the window
    
    glutSpecialFunc(special);
    

	glutDisplayFunc(display);	//registers "display" as the display callback function
	

    glutIdleFunc(idle);
    
	glEnable(GL_DEPTH_TEST);
	init();
    notesToTheTA();
    
	glutMainLoop();				//starts the event loop
    
    
	return(0);					//return may not be necessary on all compilers
}