# Documentation for libtheatre

## Stage
### Consists of:
+	 backstage
	-	Backstage is a void pointer which consists of the data that retains between scenes
+	depth
	-	The short representing depth/amount of scenes that there are
+	scene
	-	A double pointer which contains an array of the scenes in the "play"
	-	Is a double pointer so that it can be realloc'd without changing the memory address of the individual scenes
+ 	currentScene
	-	A pointer which points to the scene which is currently being played

### Initialisation:
+	malloc the stage
+	set backstage to null
+	set depth to 0
+ 	set scene to the result of the scene initialiser
+	set currentScene to null

### Freeing:
+	repeatedly free the scenes stored in scene until depth is 0
+	free stage

## Scene
### Consists of:
+	props
	-	A void pointer which points to the data that is in use for the current scene
+	update
	-	The function pointer to the scene's update/display function
+	keyboard
	-	The function pointer to the scene's keyboard/input function
+	entry
	-	The function pointer to the scene's entry/initialiser function
+	exit
	-	The function pointer to the scene's exit/free function

### Initialisation:
+	increment the stage->depth
+	realloc stage->scene to accommodate this new depth
	-	If stage->scene is null, then realloc will act the same as malloc
+	malloc the new scene
+	set each of the function pointers to their respective functions given in the arguments
+	set the currentScene to the scene if it is null

### Freeing:
+	free the scene

## Screen:
### Consists of:
+	depth
	-	The short representing depth/amount of layers that there are
+	xLength, yLength
	-	The short representing the width and height of the screen/view
+	Layer
	-	A double pointer which contains an array of the layers on the screen
	-	Is a double pointer so that it can be realloc'd without changing the memory address of the individual layers

### Initialisation
+	malloc the screen
+	set the depth to 0
+	set the x and y lengths according to the function parameters
+ 	set layer to null

### Freeing:
+	free the screen

## Layer
### Consists of:
+	draw, visibility
	-	A char which determines whether or not to draw the current layer
	-	They do the opposite of each other. 
	-	Draw is turned to true if the layer is edited
+	yOffset, xOffset
	-	A short representing what the location of the top left corner of this layer relative to the origin (top left of the terminal)
+	yLength, xLength
	-	The size of the layer in terms of y and x length
+	sprite
	-	A double pointer which contains an array of the sprites on the screen
	-	Is a double pointer since the screen is 2d

### Initialisation:
+	malloc the layer
+	set draw and visibility to true
+	set \[y,x\] \[Offset,Length\] according to their respective arguments
+	init the sprites map using the initialser

### Freeing
+	free the sprites using the free'er
+	free the layer


