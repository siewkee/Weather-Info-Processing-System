
#include "WIPS.h"

int main()
{
    int choice = 0;
    
    //variables shared by > 1 task
    int gridX_end = 0;
    int gridY_end = 0;
    string cityLocation = "";
    string cloudCover = "";
    string pressure = "";
    
    int range_X_final = 0;
    int range_Y_final = 0;
    
    Grid** Grid_2D_array = new Grid* [range_Y_final];
    
    string exit_msg = "\nPress <enter> to go back to main menu ...";
    
    do
    {
        choice = display_menu();

        if (choice == 1)
        {
            //Task A
            Task_A(gridX_end, gridY_end, cityLocation, cloudCover, pressure);
        }
        else if (choice == 2)
        {
            //Task B
            // adding extra grids for axis and perimeter
            range_X_final = gridX_end + 4;
            range_Y_final = gridY_end + 4;
            
            //creating map
            for (int i = 0; i < range_Y_final; i++)
                Grid_2D_array[i] = new Grid [range_X_final];
            
            Grid_2D_array = Task_B(Grid_2D_array, cityLocation, range_X_final, range_X_final);
            cout << exit_msg << endl;
        }
        else if (choice == 3)
        {
            //Task C
            Grid_2D_array = Task_C(cloudCover, Grid_2D_array, gridX_end, gridY_end);
            
            displayCityMap_TaskC_Index(Grid_2D_array, range_X_final, range_Y_final);

            cout << exit_msg << endl;
        }
        else if (choice == 4)
        {
            displayCityMap_TaskC_Symbol(Grid_2D_array, range_X_final, range_Y_final);
            
            cout << exit_msg << endl;
        }
        else if (choice == 5)
        {
            Grid_2D_array = Task_D(pressure, Grid_2D_array, gridX_end, gridY_end);
            displayCityMap_TaskD_Index(Grid_2D_array, range_X_final, range_Y_final);
            cout << exit_msg << endl;
            
        }
        else if (choice == 6)
        {
            displayCityMap_TaskD_Symbol(Grid_2D_array, range_X_final, range_Y_final);
            cout << exit_msg << endl;
        }
        else if (choice == 7)
        {
            Task_E(gridX_end, gridX_end, Grid_2D_array);
        }
        
        
    }while(choice != 8);
    
    //deallocate memory;
    deallocateMemory(Grid_2D_array, gridY_end);
}


