// This program uses queues and algorith strictures in order to work as a sorter of incoming orders, putting them 
// in and out the queue whenever they are ordered and delivered. It also opens and closes the restaurant according 
// to the desired action. 

#include "a1.h"

Restaurant* initialize_restaurant(char* name){ //pointing to struct
  
  //making space for the fields
  Restaurant* restaurant = (Restaurant *)malloc(sizeof(Restaurant)); 

  //assigning the values in restaurant; saying go the address of the name and assign this value
  restaurant -> name = name;
  restaurant -> menu = load_menu(MENU_FNAME); //little menu; only a place to store the whole thing
  restaurant -> num_completed_orders = 0;
  restaurant -> num_pending_orders = 0;

  //pending oredrs initialized to an empty queue
  Queue * new_queue = (Queue *)malloc(sizeof(Queue));

  new_queue -> head = NULL;
  new_queue -> tail = NULL;
  
  //initializing pending_orders to Queue with head and tail
  restaurant -> pending_orders = new_queue; 

  return restaurant;
} //do i need to free in here ?




Menu* load_menu(char* fname){
  //first, open the file with the menu fname
  FILE * fptr  = fopen(fname, "r");

  int line_len = ITEM_CODE_LENGTH + MAX_ITEM_NAME_LENGTH; //varies with each item
  char line[line_len]; //track of the # of item, info is kept here

  int count = 0;

  //count the number of lines (items)
  //fgets(str, int, file) reads a line and stores it into the string
  while(fgets(line, line_len, fptr)){ 
    count++; //number of items
  }

  //assign the space for the variables, the "house"
  char ** item_codes = (char **)malloc(count * sizeof(char *)); 
  char ** item_names = (char **)malloc(count * sizeof(char *));
  double * item_cost_per_unit = (double *) malloc(count * sizeof(double)); 

  //get the pointer from the beginning
  rewind(fptr);

  char* end_ptr; //use later for strtod

  //iterate through every line and "copy paste" the into to the variables
  for(int i = 0; i < count; i++){
    fgets(line, line_len, fptr); //fgets(str, int, file)

    /*if(i == count -1){ //i think this is used when we are about to reach the end, not sure
      line[strlen(line)] = ' ';
    }*/

    //strtok separates the string by the delimitation given, in this case MENU_DELIM
    //malloc to assign the "rooms" inside the house
  
    //creating the size of each item code
    item_codes[i] = (char*)malloc(sizeof(char) * ITEM_CODE_LENGTH);  
    strcpy(item_codes[i], strtok(line, MENU_DELIM)); //assign the line value form the file

    item_names[i] = (char*)malloc(sizeof(char) * MAX_ITEM_NAME_LENGTH);
    strcpy(item_names[i], strtok(NULL, MENU_DELIM)); //ok wtf with NULL O_o 

    //strtod converts the argument str to a floating-point number (type double) "(const char *str, char **endptr)"
    item_cost_per_unit[i] = strtod(strtok(NULL, "$"), &end_ptr);
  }
  
  fclose(fptr);

  // creating the menu with the information recovered form the file
  struct Menu * loaded_menu = (struct Menu *)malloc(sizeof(struct Menu));

  loaded_menu -> num_items = count;
  loaded_menu -> item_codes = item_codes;
  loaded_menu -> item_names = item_names;
  loaded_menu -> item_cost_per_unit = item_cost_per_unit;

  return loaded_menu;
}




Order* build_order(char* items , char* quantities){
 // Order* order = build_order("A1B1C1", "20,11,17");
 // order ->num_items; // should be 3
  int num_items; 
  
  if (items == NULL){ //if we have no items, 0 LOL
    int num_items = 0;
  }
    
  else {
    num_items = strlen(items)/(ITEM_CODE_LENGTH - 1); 
  }
  
  //making easy money with the space for these variables
  char ** item_codes = (char **)malloc(sizeof(char *) * num_items); //creating [i]
  int * item_quantities = (int *)malloc(sizeof(int) * num_items);
    
  char * temp_items = items;

  int USABLE_LENGTH = ITEM_CODE_LENGTH - 1;
  int j = 0;
  int i = 0;
  int l = 0;

  //keep doing it while all the items are added
  while(i < num_items){
    //creating the size of each item code[j]
    item_codes[i] = (char*)malloc(sizeof(char) * ITEM_CODE_LENGTH); 
    char temp_quantities[MAX_ITEM_QUANTITY_DIGITS];
    strcpy(temp_quantities, quantities);
    
    while(j < USABLE_LENGTH){ //i always increasing, always going to the next node
      item_codes[i][j] = temp_items[l];  //creating the size of each item code
      j++;
      l++; //different from j, doughnut want to set to 0 again, string continues
    }
    
    item_codes[i][USABLE_LENGTH] = '\0';
    j = 0; //need to set to 0 in order to start again to count the length on next loop

    if (i == 0){
      item_quantities[i] = atoi(strtok(temp_quantities, MENU_DELIM));
    }
      
    else {
      item_quantities[i] = atoi(strtok(NULL, MENU_DELIM));
    }
    
    i++; //next item
  }

  //creating the order and assign the info
  struct Order * order = (struct Order *)malloc(sizeof(struct Order));
  
  order -> num_items = num_items;
  order -> item_codes = item_codes;
  order -> item_quantities = item_quantities;

  return order;  
}



void enqueue_order(Order* order , Restaurant* restaurant ){

  //create the cargo of QueueNode
  struct QueueNode * NewOrder = (struct QueueNode*)malloc(sizeof(struct QueueNode));

  //initialize the values, just enough without refering to the pending orders
  NewOrder -> order = order;
  NewOrder -> next = NULL;

  //if the queue is empty
  if(restaurant -> pending_orders -> tail == NULL){
    //since is is the 1st node
    restaurant -> pending_orders -> head = NewOrder;
    restaurant -> pending_orders -> tail = NewOrder;
  }

  //how are we assigning the same varible to both?
  else {
    restaurant -> pending_orders -> tail -> next = NewOrder; //to create space? 
    restaurant -> pending_orders -> tail = NewOrder;
  }

  (restaurant -> num_pending_orders++);
}



Order* dequeue_order(Restaurant* restaurant){
  //return the dequeued order
  //create the struct for the returning pointer
  struct QueueNode* container = NULL;
  struct Order * order_1 = NULL; 

  //check if it's the last node
  if(restaurant -> pending_orders -> head == restaurant -> pending_orders -> tail){
    container = restaurant -> pending_orders -> head;
    order_1 = container -> order;
    restaurant -> pending_orders -> tail = NULL;
    restaurant -> pending_orders -> head = NULL;
  }

  else {
    container = restaurant -> pending_orders -> head;
    order_1 =  container -> order;
    restaurant -> pending_orders -> head = restaurant -> pending_orders -> head -> next;
  }

  restaurant -> num_pending_orders--;
  restaurant -> num_completed_orders++;
  free(container);
  return order_1;
}



double get_item_cost(char* item_code , Menu* menu){
  /*iterate thorugh the arrey of item_codes given by Menu, check if the item is equal 
  to the item given, if so access its cost price and return it*/
  int count = menu -> num_items;
  int i = 0;
  double cost;

  for(i = 0; i < count; i++){
    if (strcmp((menu -> item_codes[i]), item_code)==0){  
      //menu -> item_cost_per_unit)[i];
      return (menu -> item_cost_per_unit[i]); //hehe lol harcoding time
    }
  }
  
  return 0;
}



double get_order_subtotal(Order* order , Menu* menu){
  //we are given an order that we can access from the menu, we have acess to items, codes and quantities
  float subtotal = 0;
  float item_cost = 0;
  int num_items = 0;
  int i = 0;

  char item_code_1[ITEM_CODE_LENGTH];
  
  while(i < order -> num_items){
    strcpy(item_code_1, (order -> item_codes)[i]);
    item_cost = get_item_cost(item_code_1, menu);
    num_items = (order -> item_quantities)[i];
    subtotal += (num_items * item_cost);
    i++;
  }
  
  return subtotal;
}



double get_order_total(Order* order , Menu* menu){
  double total = 0;
  double taxes = 0;

  //+1 (?)
  taxes = (double)TAX_RATE/(double)100 + 1; //will round without double
  total = get_order_subtotal(order, menu) * taxes;

  return total;
}



int get_num_completed_orders(Restaurant* restaurant ){
  int completed_orders = 0;
  completed_orders = restaurant -> num_completed_orders;
  return completed_orders;
}



int get_num_pending_orders(Restaurant* restaurant ){
  int pending_orders = 0;
  pending_orders = restaurant -> num_pending_orders;
  return pending_orders;
}



void clear_order(Order ** order){
  int i = 0; 
  
  while(i < (*order) -> num_items){
    free((*order) -> item_codes[i]);
    i++;
  }
  //free(item_codes);
  //free(item_quantities);
  free((*order) -> item_codes);
  free((*order) -> item_quantities);
  free(*order);

  
  (*order) = NULL;
/*
  free((*NewOrder));
  (*NewOrder) = NULL;

  free((*order_1));
  (*order_1) = NULL;

  free((*item_code_1));*/
  //free(order);
//free Qeueue nodes
}



void clear_menu(Menu** menu){
  
  for (int i = 0; i < (*menu)-> num_items; i++){
    free((*menu) -> item_codes[i]);
    free((*menu) -> item_names[i]);
  }

  //free((*menu) -> num_items);
  free((*menu) -> item_names);
  free((*menu) -> item_codes);
  free((*menu) -> item_cost_per_unit);
  
  //free(loaded_menu);
  free((*menu));
  
  (*menu) = NULL;
  //free(menu);
}


void close_restaurant(Restaurant ** restaurant ){
  clear_menu(&((*restaurant) -> menu));
  
  //while loop to free every QueueNode
  
  while (((*restaurant) -> pending_orders -> head) != NULL){
    QueueNode* temp = (*restaurant) -> pending_orders -> head;
    (*restaurant) -> pending_orders -> head = (*restaurant) -> pending_orders -> head -> next;
    clear_order(&(temp -> order));
    free(temp);
    temp = NULL;
  }
  
  free((*restaurant) -> pending_orders);
  free(*restaurant);
  
  (*restaurant) = NULL;
  
  //free(restaurant);  
}



void print_menu(Menu* menu){
	fprintf(stdout, "--- Menu ---\n");
	for (int i = 0; i < menu->num_items; i++){
		fprintf(stdout, "(%s) %s: %.2f\n", 
			menu->item_codes[i], 
			menu->item_names[i], 
			menu->item_cost_per_unit[i]	
		);
	}
}


void print_order(Order* order){
	for (int i = 0; i < order->num_items; i++){
		fprintf(
			stdout, 
			"%d x (%s)\n", 
			order->item_quantities[i], 
			order->item_codes[i]
		);
	}
}


void print_receipt(Order* order, Menu* menu){
	for (int i = 0; i < order->num_items; i++){
		double item_cost = get_item_cost(order->item_codes[i], menu);
		fprintf(
			stdout, 
			"%d x (%s)\n @$%.2f ea \t %.2f\n", 
			order->item_quantities[i],
			order->item_codes[i], 
			item_cost,
			item_cost * order->item_quantities[i]
		);
	}
	double order_subtotal = get_order_subtotal(order, menu);
	double order_total = get_order_total(order, menu);
	
	fprintf(stdout, "Subtotal: \t %.2f\n", order_subtotal);
	fprintf(stdout, "               -------\n");
	fprintf(stdout, "Tax %d%%: \t$%.2f\n", TAX_RATE, order_total);
	fprintf(stdout, "              ========\n");
}
