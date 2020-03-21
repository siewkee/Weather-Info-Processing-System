
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>


using namespace std;
const int MAX = 100;

//enum for different grid type
enum GridTypeEnum {City_E, Perimeter_E, Axis_E, Border_E, Empty_E};

//structure for x, y coordinates
struct Coordinates
{
    int row;
    int col;
    int x;
    int y;
};

//structure for City type grids
struct City
{
    char* name;
    int cityID;
    char* content;
};

//structure for Perimter type grids; contain cityID that it is surrounding
struct Perimeter
{
    int perimeter_to_cityID;
    char* cityName;
    char* content;
};

//structure for Axis type grids; contain axis value
struct Axis
{
    int value;
};

//structure for Border type grids; contain # char
struct Border
{
    char hex_sign;
};

//structure for Empty type grids; contain empty string
struct Empty
{
    char* content;
};

// union of different possible grids
union GridTypeU
{
    Empty empty;
    City city;
    Perimeter perimeter;
    Axis axis;
    Border border;
    
};

// standard grid structure with enum and union of grid type
struct Grid
{
    GridTypeEnum type_name;
    GridTypeU type;
    Coordinates coordinates;
    int cloud_coverage;
    int cloud_index;
    char symbol;
    int pressure;
    int pressure_index;
    char pressure_symbol;
};

struct CityReport
{
    int cityID;
    char* cityName;
    int total_cloud_cov;
    int total_pressure;
    char prob_rain;
    int totalGrid;
};


//Task A
void Task_A(int&, int&, string&, string&, string&);

//General Functions for all tasks
Grid** create_initialize_2DMap(Grid* [MAX], int, int);
Grid* list_Coordinates_Axis(Grid [MAX], int, int);
Grid* list_Coordinates_Border(Grid [MAX], int, int);
Grid** update_Map(Grid* [MAX], int, Grid [MAX]);
Coordinates calculate_row_col(int, int, int);
int convert_to_integer (string);
void deallocateMemory(Grid* [MAX], int);
int display_menu();


//Task B
Grid** Task_B(Grid* [MAX], string, int, int);
int list_Coordinates_CityLoc(Grid [MAX], int, string);
vector <string> tokenizeString (string, string);
int compareByCityID(const int, const int);
void identify_Perimeter_Grids(Grid [MAX], int, Grid* [MAX], Grid [MAX]);
void displayCityMap_B(Grid* [MAX], int, int);

//Task C
Grid** Task_C(string, Grid* [MAX], int, int);
void displayCityMap_TaskC_Index(Grid* [MAX], int, int);
void displayCityMap_TaskC_Symbol(Grid* [MAX], int, int);
char cloudcov_pressure_symbol(int);

//Task D
Grid** Task_D(string, Grid* [MAX], int, int);
void displayCityMap_TaskD_Index(Grid* [MAX], int, int);
void displayCityMap_TaskD_Symbol(Grid* [MAX], int, int);

//Task E
void Task_E(int, int , Grid* [MAX]);
int compute_NumOfCity(Grid [MAX], int);
bool check_cityreport (int, int, CityReport[MAX], int, int);
int rain_prob(char, char);
void print_rain_ASCII(int);
void display_report(CityReport[MAX], int);
