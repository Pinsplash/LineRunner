# Line Runner
This program takes a CSV input and turns it into an SVG line graph. Intended to be used for creating speedrun history graphs. This was used in the making of this video: _

Making a speedrun history graph by hand is error-prone and time-consuming. Some errors existed in my graph for Half-Life. SVG (Scalable Vector Graphics) allows you to zoom in infinitely on your graph, eliminating any need to use pixelated images.

## CSV Format
If you don't have a program to make a CSV file, Google Sheets will work fine. Speedrun.com has a button to download world records in CSV format. The CSV should contain only world records, and in the order they occurred in. If there are non-record runs included, you will need to personally fix the lines drawn between them. Avoid commas and quote marks as these will lead to broken results.

To define a run, the first cell in a row should be the runner's name. The second cell should be the days past a certain point. You can decide what date to use as a reference point, but you'll probably want to go with the date of the earliest run (so the first run would use the number 0, and a run 5 days later would use 5 and so on). You can easily find the number of days between two dates with [this site](https://www.timeanddate.com/date/durationresult.html). The third cell should be the time of the run in seconds. Hours and minutes must be converted to seconds. 1m 30s must be written as 90.

Optionally, runs can be put into categories, and multiple timelines can be put into one graph as distinct categories. A row defining a category will affect all of the runs below it until there is a row defining a new category or the end of the file is reached. Runs in a category will be put in an SVG layer corresponding to the category name and will not have any lines connecting them to runs from other categories. To define a category, the first cell should contain only the word "Category" and the second cell should be the name of the category. The third cell should be left blank.

Example input:

![image](https://github.com/Pinsplash/LineRunner/assets/39359267/a349a99a-e620-44f9-9c60-4bbedd62b7a9)

## Usage
Download the program from here: _

Drag your CSV file onto linerunner.exe. You will then be asked to enter a bit of information about how you want the graph to look.

* Colors: Decide how you would like the runs to be colored. Each runner can be given their own color.
* Circle Radius and Line Width: Experiment with these. Good values will depend on how big the graph is and how far apart runs are.
* X and Y Scale: Scales the graph on the X and Y axes without distorting lines and circles. Graphs for older games will generally be very flat, so this can be used to put the graph in a more readable aspect ratio. If you want to put the graph into 16:9, create the graph once with X and Y set to 1, then solve *width/16\*9/height* to get the Y scale when X scale is 1.

## SVG
The resulting SVG file will be in the same directory as the CSV and have the same name. The file should be able to open in any vector editing program. Inkscape is free and easy to learn. From there, high quality PNGs can be rendered.

Inkscape allows you to render specific elements. A helpful trick is to make a 16:9 black box to represent the screen and put it under all other layers. Inkscape also allows you to render objects that have gone outside the border.

Example output (a few fake runs added for testing purposes):

![image](https://github.com/Pinsplash/LineRunner/assets/39359267/06bb89c6-64c0-400a-aa07-1156535f3061)
