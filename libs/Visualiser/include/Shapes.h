#pragma once

namespace Apeiron {

//void Circle(float cx, float cy, float r, int num_segments)
//{
//  float theta = 3.1415926 * 2 / float(num_segments);
//  float tangetial_factor = tanf(theta);//calculate the tangential factor
//
//  float radial_factor = cosf(theta);//calculate the radial factor
//
//  float x = r;//we start at angle = 0
//
//  float y = 0;
//  glLineWidth(2);
//  glBegin(GL_LINE_LOOP);
//  for (int ii = 0; ii < num_segments; ii++)
//  {
//    glVertex2f(x + cx, y + cy);//output vertex
//
//    //calculate the tangential vector
//    //remember, the radial vector is (x, y)
//    //to get the tangential vector we flip those coordinates and negate one of them
//
//    float tx = -y;
//    float ty = x;
//
//    //add the tangential vector
//
//    x += tx * tangetial_factor;
//    y += ty * tangetial_factor;
//
//    //correct using the radial factor
//
//    x *= radial_factor;
//    y *= radial_factor;
//  }
//  glEnd();
//}

//The function above can then be called in the main() function as such:
//drawCircle(250, 250, 100, 360);
//
//To change the circle color, you can use the glColor3f() method which takes in 3 parameters, for the RGB values.
//
//Add this line before calling the drawCircle function
//glColor3f(0.0, 0.5, 0.5);


}
