#include "../lib/SnowSence.h"

#include <cmath>
#define SNOW_RANDOM 1
#define SNOW_GRAVITY 2
#define SNOW_GRAVITY_W_WIND 3


SnowSence::SnowSence(){}
SnowSence::SnowSence(GLuint number,GLfloat size,GLfloat rage,GLfloat range){
	numberOfFlack = number;
	maxSize = size;  // size is the length of the cube
	rageLevel = rage;
	snowRange = range;
	acceleration = glm::vec3(0.0f,0.000005,0.0);
	mode = SNOW_RANDOM;
	generate();
}


void SnowSence::generate(){
	snowArray.resize(numberOfFlack);
	for (int i = 0; i < numberOfFlack; i++){
		snowArray[i].size = maxSize * ((double) rand() / (RAND_MAX));
		GLfloat theta = 2 * glm::pi<double>() * ((double) rand() / (RAND_MAX)); //0 ~ 2pi
		GLfloat fai = glm::pi<double>() * (((double) rand() / (RAND_MAX))-0.5); // -pi/2 ~ pi/2
		GLfloat posX, posY, posZ, speedX, speedY, speedZ;
		posX = snowRange * cos(theta) * cos(fai) * 0.9;
		posY = snowRange * sin(theta) * cos(fai) * 0.9;
		posZ = snowRange * sin(fai) * 0.9;
		speedX = (double(rand())/(RAND_MAX)-0.5)/500;
		speedY = (double(rand())/(RAND_MAX)-0.5)/500;
		speedZ = (double(rand())/(RAND_MAX)-0.5)/500;

		snowArray[i].pos = glm::vec3(posX,posY,posZ);
		// snowArray[i].speed = glm::vec3(0.0f,sqrt(2 * acceleration.y * (snowArray[i].pos.y+snowRange)),0.0f);
		snowArray[i].speed = glm::vec3(speedX,speedY,speedZ);
		snowArray[i].onFace=false;
		snowArray[i].color=glm::vec3(1.0f,1.0f,1.0f);
	}
	glGenBuffers(1, &SVBO);
	glGenVertexArrays(1, &VertexArrayID);


}

void SnowSence::show(GLuint center, GLuint length){
	switch(mode)
	{
		case 1:
			move_rand();
			break;
		case 2:
			move_grav();
			break;
		case 3:
			move_wind();
			break;
	}
	glBindVertexArray(VertexArrayID);
	for (int i = 0; i < numberOfFlack; i++){
		drawCube(snowArray[i],center,length);		
	}
}

glm::vec3 SnowSence::windAccelerate(glm::vec3 position){
	GLfloat windRange = snowRange*0.5;
	GLfloat distance =  length(position - glm::vec3(0.0,snowRange,0.0));
	glm::vec3 acce = normalize((position - glm::vec3(0.0,snowRange,0.0))+glm::vec3(0,-0.5,0));
	if (distance > windRange) return glm::vec3(0,0,0);
	// make it exponential with respect to distance
	GLfloat strength = exp(-distance)*0.0005;
	acce *= strength;
	return acce;
}

bool SnowSence::outRange(snowflack flack){
	GLfloat distance = length(flack.pos);
	if (distance + flack.size >= snowRange) return true;
	else return false;
}

void SnowSence::setMode(GLuint m){
	if(m==1)
		for(int i=0;i<numberOfFlack;i++)
		{
			snowArray[i].color = glm::vec3(1.0,1.0,1.0);
			if(mode==3&&snowArray[i].onFace)
			{
				snowArray[i].onFace=false;
				GLfloat speedX,speedY,speedZ;
				speedX = (double(rand())/(RAND_MAX)-0.5)/500;
				speedY = (double(rand())/(RAND_MAX)-0.5)/200;
				speedZ = (double(rand())/(RAND_MAX)-0.5)/500;
				snowArray[i].speed = glm::vec3(speedX,speedY,speedZ);
				snowArray[i].pos *= 0.99;
			}
		}
	mode = m;
}

void SnowSence::drawCube(snowflack flack,GLuint center,GLuint sideLength){
	GLfloat vertex_buffer[] = {
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	     1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};
	
	for (int i=0;i<36;i++){
		vertex_buffer[i*6]   = vertex_buffer[i*6]  *flack.size /2 + flack.pos.x;
		vertex_buffer[i*6+1] = vertex_buffer[i*6+1]*flack.size /2 + flack.pos.y;
		vertex_buffer[i*6+2] = vertex_buffer[i*6+2]*flack.size /2 + flack.pos.z;
		vertex_buffer[i*6+3] = flack.color.r;
		vertex_buffer[i*6+4] = flack.color.g;
		vertex_buffer[i*6+5] = flack.color.b;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, SVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_STATIC_DRAW);

    glBindVertexArray(VertexArrayID);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    	glEnableVertexAttribArray(1);
    
    glUniform3f(center, flack.pos.x, flack.pos.y,flack.pos.z);
    glUniform1f(sideLength, flack.size);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 12*3);
    glDisable(GL_BLEND);
    glBindVertexArray(0);

}

void SnowSence::move_rand()
{

	for(int i=0;i<numberOfFlack;i++)
	{
		snowArray[i].pos += snowArray[i].speed;
		if(outRange(snowArray[i]))
		{
			snowArray[i].pos -= snowArray[i].speed;
			snowArray[i].speed = snowArray[i].speed - 2*dot(snowArray[i].pos,snowArray[i].speed)/dot(snowArray[i].pos,snowArray[i].pos)*snowArray[i].pos;
			// snowArray[i].speed=glm::vec3(0,0,0);
		}
	}
}

void SnowSence::move_grav()
{
	for(int i=0;i<numberOfFlack;i++)
	{
		// if(snowArray[i].speed==glm::vec3(0,0,0))
		// 	continue;
		snowArray[i].speed += (acceleration*5.0f);
		snowArray[i].pos += snowArray[i].speed;
		if(outRange(snowArray[i]))
		{
			snowArray[i].pos -= snowArray[i].speed;
			if(snowArray[i].pos.y<0.2*snowRange)
				snowArray[i].speed = snowArray[i].speed - 2*dot(snowArray[i].pos,snowArray[i].speed)/dot(snowArray[i].pos,snowArray[i].pos)*snowArray[i].pos;
			else
				snowArray[i].speed -= (acceleration*5.0f);
		}
	}
}

void SnowSence::move_wind()
{
	// snowArray[i].speed += acceleration + windAccelerate(snowArray[i].pos);
	// 	snowArray[i].pos += snowArray[i].speed;
	// 	if (outRange(snowArray[i])){
	// 		snowArray[i].pos -= snowArray[i].speed;
	// 		snowArray[i].speed = glm::vec3(0,0,0);
	// 		// snowArray[i].pos 
	// 	}
	for(int i=0;i<numberOfFlack;i++)
	{
		if(!snowArray[i].onFace)
		{
			snowArray[i].pos += snowArray[i].speed;
			if(outRange(snowArray[i]))
			{
				snowArray[i].pos -= snowArray[i].speed;
				snowArray[i].onFace = true;
				snowArray[i].hitPoint = snowRange-snowArray[i].pos.y;
				snowArray[i].color.r = snowArray[i].color.g = 0.2; 
				// snowArray[i].speed = snowArray[i].speed - 2*dot(snowArray[i].pos,snowArray[i].speed)/dot(snowArray[i].pos,snowArray[i].pos)*snowArray[i].pos;
				snowArray[i].speed=glm::vec3(0,0,0);
			}
		}
		else
		{
			float oldphi = asin(snowArray[i].pos.y/snowRange);
			float newphi = oldphi + 0.005f;
			if( newphi < +glm::pi<double>()/2 )
			{
				snowArray[i].pos.y = snowRange * sin(newphi);
				snowArray[i].pos.x = snowArray[i].pos.x *cos(newphi)/cos(oldphi);
				snowArray[i].pos.z = snowArray[i].pos.z *cos(newphi)/cos(oldphi);
				snowArray[i].color.r = snowArray[i].color.g = 1 - 0.8*(snowRange-snowArray[i].pos.y)/(snowArray[i].hitPoint);
			}
			else
			{
				GLfloat speedX,speedY,speedZ;
				speedX = (double(rand())/(RAND_MAX)-0.5)/500;
				speedY = (double(rand())/(RAND_MAX)-0.5)/200;
				speedZ = (double(rand())/(RAND_MAX)-0.5)/500;
				snowArray[i].speed = glm::vec3(speedX,speedY,speedZ);
				snowArray[i].pos.y -= snowRange*0.05;
				snowArray[i].onFace = false;
				snowArray[i].color = glm::vec3(1.0,1.0,1.0);
			}
		}
	}
}


