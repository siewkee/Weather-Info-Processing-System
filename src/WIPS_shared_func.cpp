#include "WIPS.h"

int display_menu()
{
    //print name and student ID
    cout << "\nStudent ID\t: 5986606" << endl;
    cout << "Student Name\t: Hung Siew Kee" << endl;
    cout << "-------------------------------" << endl;
    cout << "Welcome to Weather Information Processing System!\n" << endl;
    
    //print menu options
    cout << "1)\tReading in and process a configuration file" << endl;
    cout << "2)\tDisplay city map" << endl;
    cout << "3)\tDisplay cloud coverage map (cloudiness index)" << endl;
    cout << "4)\tDisplay cloud coverage map (LMH symbols)" << endl;
    cout << "5)\tDisplay atmospheric pressure map (pressure index)" << endl;
    cout << "6)\tDisplay atmospheric pressure map (LMH symbols)" << endl;
    cout << "7)\tShow weather forecast summary report" << endl;
    cout << "8)\tQuit" << endl;
    
    cout << "\nPlease enter your choice: ";
    char choice_char;
    cin >> choice_char;
    
    int choice_int = choice_char - '0';
    return choice_int;
}

//creating map
//initializing map with empty grids
Grid** create_initialize_2DMap(Grid* Grid_2D_array[MAX], int range_X_final, int range_Y_final)
{
    //initializing entire map with empty grids
    for (int i = 0; i < range_Y_final; i++)
    {
        for (int j = 0; j < range_X_final; j++)
        {
            Grid_2D_array[i][j].type_name = Empty_E;
            // marking empty grids outside border with invalid cloud coverage & index
            Grid_2D_array[i][j].cloud_index = 101;
            Grid_2D_array[i][j].cloud_coverage = 101;
        }
    }
    
    
    return Grid_2D_array;
}

//store coordinates of axis value in array of coordinates struct
Grid* list_Coordinates_Axis(Grid axis_plot [MAX], int range_X_final, int range_Y_final)
{
    int array_Counter = 0;
    
    //creating array of X-axis grids
    int x_axis_value = 0;
    for (int i = 2; i < range_X_final-1; i++)
    {
        axis_plot[array_Counter].coordinates.row = range_Y_final - 1;
        axis_plot[array_Counter].coordinates.col = i;
        axis_plot[array_Counter].type_name = Axis_E;
        axis_plot[array_Counter].type.axis.value = x_axis_value;
        x_axis_value++;
        array_Counter++;
    }
    
    //creating array of Y-axis grids
    int y_axis_value = range_Y_final - 4;
    for (int i = 1; i < range_Y_final-2; i++)
    {
        axis_plot[array_Counter].coordinates.row = i;
        axis_plot[array_Counter].coordinates.col = 0;
        axis_plot[array_Counter].type_name = Axis_E;
        axis_plot[array_Counter].type.axis.value = y_axis_value;
        y_axis_value--;
        array_Counter++;
    }
    
    return axis_plot;
}

//store coordinates of map's border in array of coordinates struct
Grid* list_Coordinates_Border(Grid border_plot[MAX], int range_X_final, int range_Y_final)
{
    //listing coordinates for border at x axis
    int arrayCounter = 0;
    for (int x = 1; x < range_X_final; x++)
    {
        //border coordinates for top row
        border_plot[arrayCounter].coordinates.row = 0;
        border_plot[arrayCounter].coordinates.col = x;
        border_plot[arrayCounter].type_name = Border_E;
        border_plot[arrayCounter].type.border.hex_sign = '#';
        arrayCounter++;
        //border coordinates for bottom row
        border_plot[arrayCounter].coordinates.row = range_Y_final - 2;
        border_plot[arrayCounter].coordinates.col = x;
        border_plot[arrayCounter].type_name = Border_E;
        border_plot[arrayCounter].type.border.hex_sign = '#';
        arrayCounter++;
    }

    //listing coordinates for border at y axis
    for (int y = 1; y < range_Y_final - 2; y++)
    {
        //border coordinates for left col
        border_plot[arrayCounter].coordinates.row = y;
        border_plot[arrayCounter].coordinates.col = 1;
        border_plot[arrayCounter].type_name = Border_E;
        border_plot[arrayCounter].type.border.hex_sign = '#';
        arrayCounter++;
        //border coordinates for right col
        border_plot[arrayCounter].coordinates.row = y;
        border_plot[arrayCounter].coordinates.col = range_X_final - 1;
        border_plot[arrayCounter].type_name = Border_E;
        border_plot[arrayCounter].type.border.hex_sign = '#';
        arrayCounter++;
    }
    
    return border_plot;
}

Grid** update_Map(Grid* Grid_2D_array[MAX], int size, Grid update_Content[MAX])
{
    for (int i = 0; i < size; i++)
    {
        int row = update_Content[i].coordinates.row;
        int col = update_Content[i].coordinates.col;
        Grid_2D_array[row][col] = update_Content[i];
    }
    
    return Grid_2D_array;
}

//deallocate dynamic memory
void deallocateMemory(Grid* Grid_2D_array[MAX], int range_Y)
{
    int range_Y_final = range_Y + 4;
    for (int i = 0; i < range_Y_final; i++)
    {
        delete [] Grid_2D_array[i];
    }
    
    delete [] Grid_2D_array;
    cout << "Deleted entire 2D map" << endl;

}

Coordinates calculate_row_col(int x, int y, int range_Y)
{
    Coordinates c;
    c.col = 0 + 2 + x;
    c.row = range_Y - 3 - y;
    
    return c;
}

int convert_to_integer(string str)
{
    int result = 0;
    if (str.length() == 1)
        result = str[0] - '0';
    else if (str.length() == 2)
    {
        result = (str[0] - '0') * 10;
        result += (str[1] - '0');
    }
    else if (str.length() == 3)
    {
        result = (str[0] - '0') * 100;
        result += (str[1] - '0') * 10;
        result += (str[2] - '0');
    }
    
    return result;
}

//Task A - reading config file

//passing parameters by reference to capture data read from config file
void Task_A(int& gridX_end, int& gridY_end, string& cityLocation, string& cloudCover, string& pressure)
{
    cout << "\n[Read in and process a configuration file]" << endl;
    cout << "Please enter config filename : ";
    string inputFilename;
    cin >> inputFilename;
    
    fstream inputFile (inputFilename.c_str(), fstream::in);
    if(!inputFile)
    {
        inputFile.close();
        cout << "Failed to open " << inputFilename << " file for reading" << endl;
        exit(-1);
    }
    
    //identify keywords to search
    //reading text file line by line
    //match keywords before assigning text data to ref parameter
    string gridX_substr = "GridX_";
    string gridY_substr = "GridY_";
    string cityloc_substr = "citylocation.txt";
    char cloudc_substr[] = "cloudcover.txt";
    char press_substr[] = "pressure.txt";
    
    string aLine;
    while (getline (inputFile, aLine))
    {
        if (aLine.find(gridX_substr) != string::npos)
        {
            char digit_end = aLine.back();
            gridX_end = digit_end - '0';
            cout << "\nReading in GridX_IdxRange: 0-" << gridX_end << "... done!" << endl;
        }
        else if(aLine.find(gridY_substr) != string::npos)
        {
            char digit_end = aLine.back();
            gridY_end = digit_end - '0';
            cout << "Reading in GridY_IdxRange: 0-" << gridY_end << "... done!" << endl;
        }
        else if (aLine.find(cityloc_substr) != string::npos)
        {
            cout << "\nStore data from input file:" << endl;
            cityLocation = aLine;
            cout << cityLocation << " ... done!" << endl;
        }
        else if (aLine.find(cloudc_substr) != string::npos)
        {
            cloudCover = aLine;
            cout << cloudCover << " ... done!" << endl;
        }
        else if(aLine.find(press_substr) != string::npos)
        {
            pressure = aLine;
            cout << pressure << " ... done!" << endl;
        }
        
        inputFile.clear();
        inputFile.ignore(0, '\n');
    }
    
    cout << "\nAll records successfully stored. Going back to main menu ..." << endl;
}

//Task B - Display City Map

//Main function for task B
Grid** Task_B(Grid* Grid_2D_array[MAX], string cityLoc, int range_X_final, int range_Y_final)
{
    Grid_2D_array = create_initialize_2DMap(Grid_2D_array, range_X_final, range_Y_final);
    
    //get coordinates for axis and border
    int axis_size = range_X_final - 3 + range_Y_final - 3;
    Grid* axis_plot = new Grid [axis_size];
    axis_plot = list_Coordinates_Axis(axis_plot, range_X_final, range_Y_final);
    
    int border_size = (range_X_final - 3 + range_Y_final - 1) * 2;
    Grid* border_plot = new Grid [border_size];
    border_plot = list_Coordinates_Border(border_plot, range_X_final, range_Y_final);
    
    //update map with coordinates of axis and border
    Grid_2D_array = update_Map(Grid_2D_array, axis_size, axis_plot);
    Grid_2D_array = update_Map(Grid_2D_array, border_size, border_plot);
    
    //read citylocation file and update map with coordinates
    int est_cityLoc_size = range_X_final * range_Y_final;
    Grid* cityLoc_plot = new Grid [est_cityLoc_size];
    int cityLoc_plot_size = list_Coordinates_CityLoc(cityLoc_plot, range_Y_final, cityLoc);
    Grid_2D_array = update_Map(Grid_2D_array, cityLoc_plot_size, cityLoc_plot);
    
    //identify perimeter grids
    Grid* perimeterLoc_plot = new Grid[MAX];
    identify_Perimeter_Grids(cityLoc_plot, cityLoc_plot_size, Grid_2D_array, perimeterLoc_plot);
    
    int size_perimeterLoc = sizeof(perimeterLoc_plot)/sizeof(perimeterLoc_plot[0]);
    
    Grid_2D_array = update_Map(Grid_2D_array, size_perimeterLoc, perimeterLoc_plot);
    
    //display map
    displayCityMap_B(Grid_2D_array, range_X_final, range_Y_final);
    
    delete [] axis_plot;
    delete [] border_plot;
    delete [] perimeterLoc_plot;
    
    return Grid_2D_array;
}

//read data from cityLocation textfile
//create array of of city grids with data from text file
int list_Coordinates_CityLoc(Grid cityLoc_plot[MAX], int range_Y_final, string cityLoc)
{
    fstream inputFile (cityLoc, fstream::in);
    
    if(!inputFile)
    {
        inputFile.close();
        cout << "Failed to open " << cityLoc << " file for reading" << endl;
        exit(-1);
    }
    
    int size = 0;
    string aLine;
    
    while (getline (inputFile, aLine))
    {
        //storing city name and city id from text file
        vector <string> tokenStringVector = tokenizeString (aLine, "-");
        Grid city_g;
        city_g.type_name = City_E;
        city_g.type.city.cityID = stoi(tokenStringVector[1].c_str());
        
        string cityName = tokenStringVector[2].c_str();
        city_g.type.city.name = new char [MAX];
        strcpy(city_g.type.city.name, tokenStringVector[2].c_str());
        
        //storing coordinates from text file
        string coordinates_str = tokenStringVector [0];
        
        tokenStringVector.clear();
        tokenStringVector = tokenizeString (coordinates_str, ", ");
        string x = tokenStringVector[0];
        string y = tokenStringVector[1];
        
        tokenStringVector.clear();
        tokenStringVector = tokenizeString (x, "[");
        city_g.coordinates.x = convert_to_integer(tokenStringVector[1]);
        
        tokenStringVector.clear();
        tokenStringVector = tokenizeString (y, "]");
        city_g.coordinates.y = convert_to_integer(tokenStringVector[0]);
        
        Coordinates temp = calculate_row_col(city_g.coordinates.x, city_g.coordinates.y, range_Y_final);
        
        city_g.coordinates = temp;
        
        cityLoc_plot[size] = city_g;
        
        size++;
    }
    
    return size;
}


vector <string> tokenizeString (string input, string delimiter)
{
    size_t pos = 0;
    string token;
    vector <string> result;
    
    while ((pos = input.find(delimiter)) != string::npos)
    {
        token = input.substr(0, pos);
        result.push_back (token);
        input.erase(0, pos + delimiter.length());
    }
    
    result.push_back (input);
    return (result);
}


void identify_Perimeter_Grids(Grid cityLoc_plot[MAX], int size, Grid* Grid_2D_array [MAX], Grid perimeterLoc_plot[MAX])
{
    //for every city grid -  check if left, right, bottom, top is empty_e
    //if !city_e && !border_e = change to perimeter_e
    //if city_e = no action
    for (int i = 0; i < size; i++)
    {
        //calculate surrounding grid of each city grid
        int r_top = cityLoc_plot[i].coordinates.row - 1;
        int r_centre = cityLoc_plot[i].coordinates.row;
        int r_bottom = cityLoc_plot[i].coordinates.row + 1;
        
        int c_left = cityLoc_plot[i].coordinates.col - 1;
        int c_centre = cityLoc_plot[i].coordinates.col;
        int c_right = cityLoc_plot[i].coordinates.col + 1;
        
        char* cityName = Grid_2D_array[r_centre][c_centre].type.city.name;
        
        //check top grid
        if (Grid_2D_array[r_top][c_centre].type_name != City_E && Grid_2D_array[r_top][c_centre].type_name != Border_E)
        {
            Grid_2D_array[r_top][c_centre].type_name = Perimeter_E;
            Grid_2D_array[r_top][c_centre].type.perimeter.perimeter_to_cityID = cityLoc_plot[i].type.city.cityID;
            Grid_2D_array[r_top][c_centre].type.perimeter.cityName = cityName;
        }
        
        //check diagonal top left
        if (Grid_2D_array[r_top][c_left].type_name != City_E && Grid_2D_array[r_top][c_left].type_name != Border_E)
        {
            Grid_2D_array[r_top][c_left].type_name = Perimeter_E;
            Grid_2D_array[r_top][c_left].type.perimeter.perimeter_to_cityID = cityLoc_plot[i].type.city.cityID;
            Grid_2D_array[r_top][c_left].type.perimeter.cityName = cityName;
        }
        
        //check diagonal top right
        if (Grid_2D_array[r_top][c_right].type_name != City_E && Grid_2D_array[r_top][c_right].type_name != Border_E)
        {
            Grid_2D_array[r_top][c_right].type_name = Perimeter_E;
            Grid_2D_array[r_top][c_right].type.perimeter.perimeter_to_cityID = cityLoc_plot[i].type.city.cityID;
            Grid_2D_array[r_top][c_right].type.perimeter.cityName = cityName;
        }
        
        //check left
        if (Grid_2D_array[r_centre][c_left].type_name != City_E && Grid_2D_array[r_centre][c_left].type_name != Border_E)
        {
            Grid_2D_array[r_centre][c_left].type_name = Perimeter_E;
            Grid_2D_array[r_centre][c_left].type.perimeter.perimeter_to_cityID = cityLoc_plot[i].type.city.cityID;
            Grid_2D_array[r_centre][c_left].type.perimeter.cityName = cityName;
        }
        
        //check right
        if (Grid_2D_array[r_centre][c_right].type_name != City_E && Grid_2D_array[r_centre][c_right].type_name != Border_E)
        {
            Grid_2D_array[r_centre][c_right].type_name = Perimeter_E;
            Grid_2D_array[r_centre][c_right].type.perimeter.perimeter_to_cityID = cityLoc_plot[i].type.city.cityID;
            Grid_2D_array[r_centre][c_right].type.perimeter.cityName = cityName;
        }
        
        //check diagonal bottom left
        if (Grid_2D_array[r_bottom][c_left].type_name != City_E && Grid_2D_array[r_bottom][c_left].type_name != Border_E)
        {
            Grid_2D_array[r_bottom][c_left].type_name = Perimeter_E;
            Grid_2D_array[r_bottom][c_left].type.perimeter.perimeter_to_cityID = cityLoc_plot[i].type.city.cityID;
            Grid_2D_array[r_bottom][c_left].type.perimeter.cityName = cityName;
        }
        
        //check bottom
        if (Grid_2D_array[r_bottom][c_centre].type_name != City_E && Grid_2D_array[r_bottom][c_centre].type_name != Border_E)
        {
            Grid_2D_array[r_bottom][c_centre].type_name = Perimeter_E;
            Grid_2D_array[r_bottom][c_centre].type.perimeter.perimeter_to_cityID = cityLoc_plot[i].type.city.cityID;
            Grid_2D_array[r_bottom][c_centre].type.perimeter.cityName = cityName;
        }
        
        //check diagonal bottom right
        if (Grid_2D_array[r_bottom][c_right].type_name != City_E && Grid_2D_array[r_bottom][c_right].type_name != Border_E)
        {
            Grid_2D_array[r_bottom][c_right].type_name = Perimeter_E;
            Grid_2D_array[r_bottom][c_right].type.perimeter.perimeter_to_cityID = cityLoc_plot[i].type.city.cityID;
            Grid_2D_array[r_bottom][c_right].type.perimeter.cityName = cityName;
        }
        
    }
}

void displayCityMap_B(Grid* Grid_2D_array[MAX], int range_X_final, int range_Y_final)
{
    for (int i = 0; i < range_Y_final; i++)
    {
        for (int j = 0; j < range_X_final; j++)
        {
            switch (Grid_2D_array[i][j].type_name)
            {
                case City_E:
                    cout << Grid_2D_array[i][j].type.city.cityID << " ";
                    break;
                case Axis_E:
                    cout << Grid_2D_array[i][j].type.axis.value << " ";
                    break;
                case Border_E:
                    cout << Grid_2D_array[i][j].type.border.hex_sign << " ";
                    break;
                case Empty_E:
                    cout << " ";
                    break;
                case Perimeter_E:
                    cout << "  ";
                    break;
            }
        }
        cout << endl;
    }
    
    cout <<  endl;
}

Grid** Task_C(string cloudCover, Grid* Grid_2D_array[MAX], int rangeX, int rangeY)
{
    
    int range_Y_final = rangeY + 4;
    //int range_X_final = rangeX + 4;
    
    fstream inputFile (cloudCover, fstream::in);
    
    if(!inputFile)
    {
        inputFile.close();
        cout << "Failed to open " << cloudCover << " file for reading" << endl;
        exit(-1);
    }
    
    string aLine;
    
    while (getline (inputFile, aLine))
    {
        //storing cloud coverage from text file
        vector <string> tokenStringVector = tokenizeString (aLine, "-");
        string cloud_cov_str = tokenStringVector[1];
        int cloud_cov_int = convert_to_integer(cloud_cov_str);
        
        //storing coordinates from text file
        string coordinates_str = tokenStringVector [0];
        
        tokenStringVector.clear();
        tokenStringVector = tokenizeString (coordinates_str, ", ");
        string x_str = tokenStringVector[0];
        string y_str = tokenStringVector[1];
        
        tokenStringVector.clear();
        tokenStringVector = tokenizeString (x_str, "[");
        int x = convert_to_integer(tokenStringVector[1]);
        
        tokenStringVector.clear();
        tokenStringVector = tokenizeString (y_str, "]");
        int y = convert_to_integer(tokenStringVector[0]);
        
        Coordinates temp = calculate_row_col(x, y, range_Y_final);
        
        //only insert cloud value if it is a city / perimeter / empty grid
        bool check = true;
        if (Grid_2D_array[temp.row][temp.col].type_name == Axis_E || Grid_2D_array[temp.row][temp.col].type_name == Border_E)
            check = false;
        
        if (check)
        {
            Grid_2D_array[temp.row][temp.col].cloud_coverage = cloud_cov_int;
            Grid_2D_array[temp.row][temp.col].cloud_index = (int)cloud_cov_int / 10;
            
            Grid_2D_array[temp.row][temp.col].symbol = cloudcov_pressure_symbol(Grid_2D_array[temp.row][temp.col].cloud_coverage);
        }
    }
    
    return Grid_2D_array;
}


void displayCityMap_TaskC_Index(Grid* Grid_2D_array[MAX], int range_X_final, int range_Y_final)
{
    for (int i = 0; i < range_Y_final; i++)
    {
        for (int j = 0; j < range_X_final; j++)
        {
            switch (Grid_2D_array[i][j].type_name)
            {
                case Axis_E:
                    cout << Grid_2D_array[i][j].type.axis.value << " ";
                    break;
                case Border_E:
                    cout << Grid_2D_array[i][j].type.border.hex_sign << " ";
                    break;
                case Empty_E:
                    if (Grid_2D_array[i][j].cloud_index > 100)
                        cout << "  ";
                    else
                        cout << Grid_2D_array[i][j].cloud_index << " ";
                    break;
                case City_E:
                case Perimeter_E:
                    cout << Grid_2D_array[i][j].cloud_index << " ";
                    break;
            }
        }
        cout << endl;
    }
    
    cout <<  endl;
}

void displayCityMap_TaskC_Symbol(Grid* Grid_2D_array[MAX], int range_X_final, int range_Y_final)
{
    for (int i = 0; i < range_Y_final; i++)
    {
        for (int j = 0; j < range_X_final; j++)
        {
            switch (Grid_2D_array[i][j].type_name)
            {
                case Axis_E:
                    cout << Grid_2D_array[i][j].type.axis.value << " ";
                    break;
                case Border_E:
                    cout << Grid_2D_array[i][j].type.border.hex_sign << " ";
                    break;
                case Empty_E:
                    if (Grid_2D_array[i][j].cloud_index > 100)
                        cout << "  ";
                    else
                        cout << Grid_2D_array[i][j].symbol << " ";
                    break;
                case City_E:
                case Perimeter_E:
                    cout << Grid_2D_array[i][j].symbol << " ";
                    break;
            }
        }
        cout << endl;
    }
    
    cout <<  endl;
}

char cloudcov_pressure_symbol(int x)
{
    if (x < 35)
        return 'L';
    else if (x <65)
        return 'M';
    else if (x < 100)
        return 'H';
    else
        return ' ';
}

Grid** Task_D(string pressure, Grid* Grid_2D_array[MAX], int rangeX, int rangeY)
{
    int range_Y_final = rangeY + 4;
    
    fstream inputFile (pressure, fstream::in);
    
    if(!inputFile)
    {
        inputFile.close();
        cout << "Failed to open " << pressure << " file for reading" << endl;
        exit(-1);
    }
    
    string aLine;
    
    while (getline (inputFile, aLine))
    {
        //storing cloud coverage from text file
        vector <string> tokenStringVector = tokenizeString (aLine, "-");
        string pressure_str = tokenStringVector[1];
        int pressure_int = convert_to_integer(pressure_str);
        
        //storing coordinates from text file
        string coordinates_str = tokenStringVector [0];
        
        tokenStringVector.clear();
        tokenStringVector = tokenizeString (coordinates_str, ", ");
        string x_str = tokenStringVector[0];
        string y_str = tokenStringVector[1];
        
        tokenStringVector.clear();
        tokenStringVector = tokenizeString (x_str, "[");
        int x = convert_to_integer(tokenStringVector[1]);
        
        tokenStringVector.clear();
        tokenStringVector = tokenizeString (y_str, "]");
        int y = convert_to_integer(tokenStringVector[0]);
        
        Coordinates temp = calculate_row_col(x, y, range_Y_final);
        
        //only insert cloud value if it is a city / perimeter / empty grid
        bool check = true;
        if (Grid_2D_array[temp.row][temp.col].type_name == Axis_E || Grid_2D_array[temp.row][temp.col].type_name == Border_E)
            check = false;
        
        if (check)
        {
            Grid_2D_array[temp.row][temp.col].pressure = pressure_int;
            Grid_2D_array[temp.row][temp.col].pressure_index = (int)pressure_int / 10;
            
            Grid_2D_array[temp.row][temp.col].pressure_symbol = cloudcov_pressure_symbol(Grid_2D_array[temp.row][temp.col].pressure);
        }
    }
    
    return Grid_2D_array;
}


void displayCityMap_TaskD_Index(Grid* Grid_2D_array[MAX], int range_X_final, int range_Y_final)
{
    for (int i = 0; i < range_Y_final; i++)
    {
        for (int j = 0; j < range_X_final; j++)
        {
            switch (Grid_2D_array[i][j].type_name)
            {
                case Axis_E:
                    cout << Grid_2D_array[i][j].type.axis.value << " ";
                    break;
                case Border_E:
                    cout << Grid_2D_array[i][j].type.border.hex_sign << " ";
                    break;
                case Empty_E:
                    if (Grid_2D_array[i][j].cloud_index > 100)
                        cout << "  ";
                    else
                        cout << Grid_2D_array[i][j].pressure_index << " ";
                    break;
                case City_E:
                case Perimeter_E:
                    cout << Grid_2D_array[i][j].pressure_index << " ";
                    break;
            }
        }
        cout << endl;
    }
    
    cout <<  endl;
}

void displayCityMap_TaskD_Symbol(Grid* Grid_2D_array[MAX], int range_X_final, int range_Y_final)
{
    for (int i = 0; i < range_Y_final; i++)
    {
        for (int j = 0; j < range_X_final; j++)
        {
            switch (Grid_2D_array[i][j].type_name)
            {
                case Axis_E:
                    cout << Grid_2D_array[i][j].type.axis.value << " ";
                    break;
                case Border_E:
                    cout << Grid_2D_array[i][j].type.border.hex_sign << " ";
                    break;
                case Empty_E:
                    if (Grid_2D_array[i][j].cloud_index > 100)
                        cout << "  ";
                    else
                        cout << Grid_2D_array[i][j].pressure_symbol << " ";
                    break;
                case City_E:
                case Perimeter_E:
                    cout << Grid_2D_array[i][j].pressure_symbol << " ";
                    break;
            }
        }
        cout << endl;
    }
    
    cout <<  endl;
}

void Task_E(int range_X, int range_Y, Grid* Grid_2D_array[MAX])
{
    int range_X_final = range_X + 4;
    int range_Y_final = range_Y + 4;
    
    //assume that total no of cities will not be more than total of x & y range
    int est_numOfCities = range_X_final + range_Y_final;
    
    CityReport* report = new CityReport[est_numOfCities];
    int reportCounter = 0;
    
    for (int i = 0; i < range_Y_final; i++)
    {
        for (int j = 0; j < range_X_final; j++)
        {
            int cloud_cov = Grid_2D_array[i][j].cloud_coverage;
            int pressure = Grid_2D_array[i][j].pressure;
            
            // for every city/perimeter grid, check if report struct has been created for the city ID
            // if created - add on the cloud coverage and pressure
            // if !created - create report struct for the city ID
            if (Grid_2D_array[i][j].type_name == Perimeter_E)
            {
                bool created = check_cityreport (est_numOfCities, Grid_2D_array[i][j].type.perimeter.perimeter_to_cityID, report, cloud_cov, pressure);
                
                if (!created)
                {
                    CityReport r;
                    r.total_cloud_cov = cloud_cov;
                    r.total_pressure = pressure;
                    r.totalGrid = 1;
                    r.cityName = Grid_2D_array[i][j].type.perimeter.cityName;
                    r.cityID = Grid_2D_array[i][j].type.perimeter.perimeter_to_cityID;
                    
                    report[reportCounter] = r;
                    reportCounter++;
                }
                
            }
            else if (Grid_2D_array[i][j].type_name == City_E)
            {
                bool created = check_cityreport (est_numOfCities, Grid_2D_array[i][j].type.city.cityID, report, cloud_cov, pressure);
                
                if (!created)
                {
                    CityReport r;
                    r.total_cloud_cov = cloud_cov;
                    r.total_pressure = pressure;
                    r.totalGrid = 1;
                    r.cityName = Grid_2D_array[i][j].type.perimeter.cityName;
                    r.cityID = Grid_2D_array[i][j].type.perimeter.perimeter_to_cityID;
                    
                    report[reportCounter] = r;
                    reportCounter++;
                }
            }
        }
    }
    
    display_report(report, reportCounter);
}

//check if report struct has been created for city
//if already created, add ave cloud and pressure
bool check_cityreport (int NumOfCity, int ID, CityReport report [MAX], int cloud_cov, int pressure)
{
    bool created = false;
    for (int r = 0; r < NumOfCity; r++)
    {
        if(ID == report[r].cityID)
        {
            created = true;
            report[r].total_cloud_cov += cloud_cov;
            report[r].total_pressure += pressure;
            report[r].totalGrid++;
        }
    }
    
    return created;
}

void display_report(CityReport report [MAX], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << fixed << showpoint << setprecision (2);
        
        cout << left << setw (10) << "City Name: "
        << left << setw (10) << report[i].cityName << endl;
        
        cout << left << setw (10) << "City ID "
        << left << setw (10) << report[i].cityID << endl;
        
        float average_cloud_cov = report[i].total_cloud_cov / report[i].totalGrid;
        
        float average_pressure = report[i].total_pressure / report[i].totalGrid;
        
        char cloud_cov_sym = cloudcov_pressure_symbol(average_cloud_cov);
        char pressure_sym = cloudcov_pressure_symbol(average_pressure);
        
        cout << left << setw (22) << "Ave. Cloud Cover"
        << left << setw(8) << "(ACC)"
        << left << setw (3) << ":" << average_cloud_cov
        << left << setw(1) << " (" << cloud_cov_sym << ")" << endl;
        
        cout << left << setw (22) << "Ave. Pressure"
        << left << setw(8) << "(AP)"
        << left << setw (3) << ":" << average_pressure
        << left << setw(1) << " (" << pressure_sym << ")" << endl;
        
        int rain_probability = rain_prob(cloud_cov_sym, pressure_sym);
        
        cout << left << setw (22) << "Probability of Rain"
        << left << setw(8) << "(%)"
        << left << setw (5) << ":" << rain_probability << endl;
        
        print_rain_ASCII(rain_probability);
        
        cout << endl;
        
    }
}

int rain_prob(char cloud, char pressure)
{
    if (pressure == 'L')
    {
        if(cloud == 'H')
            return 90;
        else if (cloud == 'M')
            return 80;
        else if (cloud == 'L')
            return 70;
        else
            return 0;
    }
    else if (pressure == 'M')
    {
        if(cloud == 'H')
            return 60;
        else if (cloud == 'M')
            return 50;
        else if (cloud == 'L')
            return 40;
        else
            return 0;
    }
    else if (pressure == 'L')
    {
        if(cloud == 'H')
            return 30;
        else if (cloud == 'M')
            return 20;
        else if (cloud == 'L')
            return 10;
        else
            return 0;
    }
    else
        return 0;
}

void print_rain_ASCII(int rain_p)
{
    switch (rain_p)
    {
        case 90:
            cout << left << setw (4) << "~~~~" << endl;
            cout << left << setw (5) << "~~~~~" << endl;
            cout << right << setw (5) << "\\\\\\\\\\" << endl;
            break;
        case 80:
            cout << left << setw (4) << "~~~~" << endl;
            cout << left << setw (5) << "~~~~~" << endl;
            cout << right << setw (5) << "\\\\\\\\" << endl;
            break;
        case 70:
            cout << left << setw (4) << "~~~~" << endl;
            cout << left << setw (5) << "~~~~~" << endl;
            cout << right << setw (5) << "\\\\\\" << endl;
            break;
        case 60:
            cout << left << setw (4) << "~~~~" << endl;
            cout << left << setw (5) << "~~~~~" << endl;
            cout << right << setw (5) << "\\\\" << endl;
            break;
        case 50:
            cout << left << setw (4) << "~~~~" << endl;
            cout << left << setw (5) << "~~~~~" << endl;
            cout << right << setw (5) << "\\" << endl;
            break;
        case 40:
            cout << left << setw (4) << "~~~~" << endl;
            cout << left << setw (5) << "~~~~~" << endl;
            cout << right << setw (5) << "" << endl;
            break;
        case 30:
            cout << left << setw (4) << "~~~" << endl;
            cout << left << setw (5) << "~~~~" << endl;
            cout << right << setw (5) << "" << endl;
            break;
        case 20:
            cout << left << setw (4) << "~~" << endl;
            cout << left << setw (5) << "~~~" << endl;
            cout << right << setw (5) << "" << endl;
            break;
        case 10:
            cout << left << setw (4) << "~" << endl;
            cout << left << setw (5) << "~~" << endl;
            cout << right << setw (5) << "" << endl;
            break;
    }
}
