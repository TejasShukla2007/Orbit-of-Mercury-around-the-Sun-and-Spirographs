

#include <iostream>
#include <raylib.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <vector>
#include <utility>

#define AU_in_km 1.496e8
using namespace std;

Color trailColor = {200, 200, 200, 80};  // RGBA (light gray with alpha = 80)
Color sunColor = {255,154,0,255};
class Body;
double accx(Body,Body,double);
double accy(Body,Body,double);
class Body{
    public:
        double x,y;
        int r, mass;
        double v_x,v_y;
        double a_x,a_y;
        Color b_color;
        void draw(){
            DrawCircle((int)x,(int)y,r,b_color);
        }

        void update(double t, Body& other,double G){
            double old_ax = a_x;
            double old_ay = a_y;

            x+=v_x*t + (old_ax*t*t)/2;
            y+=v_y*t + (old_ay*t*t)/2;
            a_x = accx(*this,other,G);
            a_y = accy(*this,other,G);
            v_x += 0.5*(a_x+old_ax)*t;
            v_y += 0.5*(a_y+old_ay)*t;
            
            

        }
};

Body body1;
Body body2;

double dist(Body body1,Body body2){
    return sqrt((body1.x-body2.x)*(body1.x-body2.x) + (body1.y-body2.y)*(body1.y-body2.y) );
}

//Properties between the bodies

double accx(Body body1, Body body2, double G){
    double d12 = dist(body1,body2);
    return ((body2.mass*(body2.x-body1.x))/(d12*d12*d12))*G;
}
double accy(Body body1, Body body2, double G){
    double d12 = dist(body1,body2);
    return ((body2.mass*(body2.y-body1.y))/(d12*d12*d12))*G;
}
vector<pair<int, int>> path_body2;

//Unit system
int au_px = 700; // 1 AU = 800px;
double px_km = AU_in_km/au_px; // 1px in km
double time_period = 7.6032e6; //in seconds
double sim_s = time_period/5; // 1 sim_s in seconds
double  speed = 59; //in km/s;
double sim_speed = (speed*sim_s)/(px_km);
double ms = 1.989e30; // 1 sun unit = 1.989e30 kg;
double G = (6.67e-20*ms*sim_s*sim_s)/(px_km*px_km*px_km);
int main(){
    
    cout<<"Three bodies problem"<<endl;
    
    const int screen_width = 2500, screen_height = 1500;
    InitWindow(screen_width,screen_height,"Sim");
    SetTargetFPS(60);

    body1.x = 1200;
    body1.y = 750;
    body1.r = 40;
    body1.b_color = sunColor;
    body1.v_x = 0;
    body1.v_y = 0;
    body1.mass = 1;
    body1.a_x = accx(body1,body2,G);
    body1.a_y = accy(body1,body2,G);
    

    body2.x = 1200-(0.307499*au_px);
    body2.y = 750;
    body2.r = 20;
    body2.b_color = GRAY;
    body2.v_x = 0;
    body2.v_y = sim_speed;
    body2.mass = 3.285e23/ms;
    body2.a_x = accx(body2,body1,G);
    body2.a_y = accy(body2,body1,G);

    
    
    while(WindowShouldClose()==false){
        
        
        BeginDrawing();
        
        //Drawing
        ClearBackground(BLACK);
        body1.draw();
        body2.draw();


        double dt = 0.005;  // Fixed physics step
        double time_scale = 15.0; //threshold is 101, 95 is beautiful
        double sim_dt = dt * time_scale;

        
        int physics_steps_per_frame = 3;
        for (int i = 0; i < physics_steps_per_frame; ++i) {
            body1.update(sim_dt, body2,G);
            body2.update(sim_dt, body1,G);
        }

        path_body2.push_back({(int)body2.x, (int)body2.y});
        
        for (size_t i = 1; i < path_body2.size(); ++i) {
            DrawLine(path_body2[i - 1].first, path_body2[i - 1].second,
                    path_body2[i].first, path_body2[i].second, trailColor);
        }



        
        if((round(body2.y)<755)&&(round(body2.y)>745)){
            cout<<(body2.x-body1.x)<<endl;
        }
        
        EndDrawing();
    }
    return 0;
}