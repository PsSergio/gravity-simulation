#ifndef CIRCLE_H
#define CIRCLE_H

#include "../glad/glad.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include<time.h>

class Circle{
  unsigned int VAO, VBO, EBO;
  int res;
  float centerx , centery, radius; 
  float mass;
  float velocity[2] = {0.0f, 0.0f};
  float aceleration[2] = {0.0f, 10.0f};
  float centerxInitial, centeryInital;

  float tFinal;
  float tInitial = 0.0f;

  bool changeYdir = false;

  public:

    Circle(int _res, float _centerx, float _centery, float _radius){
      res = _res;
      centerx = _centerx;
      centery = _centery;
      radius = _radius;

      centerxInitial = centerx;
      centeryInital = centery;

      setVertexs();

      tFinal = sqrt( pow(velocity[1], 2) + 2 * aceleration[1] * (centery + 1 - radius) ) / aceleration[1];

      std::cout << "tFinal: " << tFinal << std::endl;
    }

    void changePosition(){
      
      if (!changeYdir){
        tInitial += 0.01f;
        if(tInitial >= tFinal) changeYdir = true;
      }else{
        tInitial -= 0.01f;
        if(tInitial <= 0.0f) changeYdir = false;
      }



      centery = centeryInital - ( ( aceleration[1] * pow(tInitial, 2) ) / 2 );

      std::cout << "centery: " << centery << std::endl;
      std::cout << "tInital: " << tInitial << std::endl;

    }



    void use(){

      setVertexs();

      glBindVertexArray(VAO);

      glDrawElements(GL_TRIANGLES, res*3, GL_UNSIGNED_INT, 0);

      // std::cout <<  centerx << std::endl;

    }

    void deleteBuffers(){
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &EBO);
    }

    float calculateGForce(float otherMass, float distance){
      float G = 6.67 * pow(10, -11);

      return ( G * mass * otherMass ) / (distance * distance);
    }

  private:
    void setVertexs(){
      int count = 3;
      float angleRad = 2.0f * 3.14159265359 / res;

      // std::cout << "angle: " << angleRad << std::endl;
      
      int indices[res*3];
      
      float vertices[(res+1)*3];

      float x = (cos(angleRad) * radius) + centerx;
      float y = (sin(angleRad) * radius) + centery;
      float z = 0.0f;

      // std::cout << "coordinates (x, y, z): " << x << " " << y << " " << z << "\n" << std::endl;

      vertices[0] = centerx;
      vertices[1] = centery;
      vertices[2] = z;

      for(int i = 0; i < res; i++){

        vertices[count] = x;
        vertices[count+1] = y;
        vertices[count+2] = z;

        // std::cout << "old coordinates: " << x << " " << y << std::endl;

        x = (cos(i*angleRad) * radius) + centerx;
        y = (sin(i*angleRad) * radius) + centery;

        count = count + 3;


        // std::cout << "new coordinates: " << x << " " << y << std::endl;
      
      }

      count = 0;
      int aux = 1;
      for(int i = 0; i < res; i++){
        
        indices[count] = 0;

      
        if(aux == res){
          indices[count+1] = aux;
          indices[count+2] = 1;  
        }else{
          indices[count+1] = aux;
          indices[count+2] = aux+1;

        }
        

        count = count+3;
        aux = aux + 1;
      }

      // std::cout << "vertices number: " << sizeof(vertices)/sizeof(float) << std::endl;
      
      // showVertices(vertices);

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);

    }

    void showVertices(float vertices[]){
      int size = (res*3*3);
      int aux = 1;
      for(int i = 0; i < size; i=i+3){
        std::cout << vertices[i] << " " << vertices[i+1] << " " << vertices[i+2] << std::endl;

        if(aux%3 == 0 && i != 0) {
          std::cout << "------------------------------------" << std::endl;
        }
        aux++;
      }
    }

};

#endif