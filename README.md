## OVERVIEW
This project supports the Barrel Timing Layer (BTL) assembly project. The aim is 
to measure the relative humidity and temperature in the assembly rooms where
sensor modules (SMs) are being cured to understand better these environments and
ensure that our SMs are of optimal quality.

We use Arduino Elegoo MEGA2560 R3 and DUE boards with fully calibrated ASAIR
AHT10 humidity and temperature sensors connected to them to gather the data.
The Arduino code is written in C++ and, once run, sends its data to a script
written in Python, which receives the data from the Arduinos and organizes
them into text files. Then, we use another Python script to plot the data.
A detailed description of the project can be found in
`Guinto-Brody_BTL_Humidity_Sensor_Project_2024.pdf`.


## REPOSITORY CONTENTS
### Python Scripts
`sensorData.py` --- Takes sensor data from serial port and writes it to data 
file  
`weatherData.py` --- Fetches weather data from Charlottesville using WTTR and
writes it to separate data file  
`NEW_READER.py` --- An updated plotting script that uses pandas; can plot an 
arbitrary number of data files (in a new format) in any order and can plot 
updating data

### Sensor Files (Contained in `aht10` Folder)
`aht10.ino` --- Initializes the sensor, measures humidity and temperature in 
room, and sends information to serial port  
`AHTxx.cpp` --- Arduino library  
`AHTxx.h`   --- Header file for Arduino library

### Example Data and Plots (Contained in `examples` Folder)
`EXAMPLE_hums_graph.png` `EXAMPLE_temps_graph.png` --- Example graphs in the 
newer format  
`2024-07-01_at_10:39:43_to_2024-07-02_at_12:38:05.png` --- Example graph in 
the newest format (both humidities and temperatures are plotted on the 
same graph with precipitation on temperature subplot and absolute humidity
on humidity subplot)    
`EXAMPLE_sensor_0.txt` `EXAMPLE_weather.txt` `EXAMPLE_sensor_1.txt` --- Data 
files taken in the new format

### Miscellaneous/Housekeeping
`README.md` --- Contains information about project and how to use programs  
`Guinto-Brody_BTL_Humidity_Sensor_Project_2024.pdf` --- Report about project  
`.gitignore.txt` --- Contains files that are not tracked by Git  
`.gitattributes` --- Used for pushing files that are larger than the maximum 
allowed size for pushing


## USING `aht10.ino`
The first section of `aht10.ino` initializes the sensor to do the following:
1. Begin collecting data after 9600 ms.
2. Delay operation by 4800 ms.
3. Print "Starting up..."
4. Check to see if the sensor is running and, if not, delay the data collection 
by 5000 ms and print "Sensor not running."
5. Print "AHT10 running" and set the cycle mode if the sensor is running.  

The line `delay(4800)` was added March 21, 2025 to allow two different Arduino
boards to be used simultaneously.

The second section of the program is an infinite loop that collects measurements
 indefinitely via the following:
1. Define the variable "humidity" to be the humidity measured by the sensor. The
 definition accesses a function in `aht10` that measures the humidity. 
2. Define the variable "temperature" in the same way but pass through the 
respective function the opposite Boolean so that the measurements are separate.
3. Print the values to the serial monitor.
4. Get the next measurement after a certain number of ms (in our case, after
 1000 ms).

Depending on the PC used (i.e. an old Centos7 system, Geekom), different
commands must be used to execute the Arduino from the command line.

### Old CentOS7 System
To execute `aht10.ino`, use the following command: 

`arduino --upload --port [SERIAL PORT] [PROGRAM].ino`

The components of the command are the following:
1. `arduino` --- The executing function
2. `--upload` --- Builds and compiles the program for use
3. `--port` --- Signals to the program to print to the serial port. If it's not 
specified, it will use the last used port, so it's good to specify so we print
to the port we want
4. `[SERIAL PORT]` --- The serial port to which we print. We use `/dev/ttyACM0`,
but that only depends on which COM port is being used. For the machine being
used, we can change the number after "ACM" to switch the port
5. `[PROGRAM].ino` --- The Arduino program. Ours is `aht10.ino`

NOTE: When you execute this command, the sensor will *not* appear to be 
collecting measurements; however, it is. Check the serial monitor on the Arduino
 interface to be sure, or execute `sensorData.py` to see the values printed to 
the screen.

NOTE: `aht10.ino` can collect measurements indefinitely or after a set amount of
 time. This can be done by modifying the code so that the appropriate sections 
are or aren't commented out. The user can also modify how many points per second
 are collected and how long they want to collect data for, if this is how they 
want to use the program.

NOTE: This code can be executed using the Arduino IDE, as well. In fact, this 
may be the optimal method of running the Arduino. However, I like running it 
from the command line to keep everything in the command line. It is important to
 note that to do this, `aht10.ino`, `AHTxx.cpp`, and `AHT10xx.h` must be 
contained in a folder. Otherwise, the code will not work.

### Geekom PC
First, the Arduino client `arduino-cli` must be installed onto the PC and placed
 in the directory where the code will be run (or in a directory in the directory
 where the code will be run). Once it is installed, we can compile and upload
the Arduino script using the following commands:

`./[PATH IF NEEDED]/arduino-cli compile -b [BOARD NAME] [FILE PATH]/[FILE NAME]`  
`./[PATH IF NEEDED]/arduino-cli upload -p [PORT] [FILE PATH]/[FILE NAME]`

or

`./[PATH IF NEEDED]/arduino-cli upload [FILE PATH]/[FILE NAME] -p [PORT] -b [BOARD NAME]`

The flag `-p` stands for "port," which is used above. The flag `-b` stands for
"board," which is the Arduino board recognized by the IDE (if the board isn't 
recognized, it must be installed). One can find the board name of the Arduino by
 running the following command:

`./arduino-cli board list`

The board name will be under the "FQBN" column, which stands for "Fully 
Qualified Board Name." The file path and name are self explanatory.

NOTE: As of 6/13/2024, I discovered that any changes you make to `aht10.ino`
will *not* be registered until you compile and upload the script using the
Arduino IDE. Since we're not changing `aht0.ino` enough for this to occur, I
am not worrying too hard about it.

NOTE: As of 7/15/2025, to switch between different boards, you must execute both
the "compile" and "upload" commands. The single command does not seem to work
when using multiple Arduino boards.


## USING `sensorData.py`
`sensorData.py` is used with the following command:

`python sensorData.py [INTEGER] [DATA FILE].txt`
1. `python` --- The executing function
2. `sensorData.py` --- The Python program
3. `[INTEGER]` --- A number telling the program which serial port to use
4. `[DATA FILE].txt` --- Includes measurements from the sensor

To reduce confusion, name sensor data files with the following format:

`sensor[# OF SENSOR BASED ON SERIAL PORT]_[ABBREVIATION OF MONTH WHEN DATA COLLECTION STARTS][# OF DAY WHEN DATA COLLECTION STARTS].txt`

After the program is run, it will print whatever is sent to the serial port by
`aht10.ino` to the screen. Those values, as well as the date and time of the 
measurement and an index used for plotting, are then written to the first data 
file (they're actually appended so no data is lost).

5/24/2024 UPDATE: Instead of plotting an index, `sensorData.py` now plots the 
serial port number in the first column. This is useful for using 
`NEW_READER.py`.

NOTE: It appears `serial` only works with root access, so the user must be
logged in as a root user to execute the function. This can be accomplished by
running the following command:

`sudo su`

The user will then be prompted to enter their password. After that is entered, 
the program should be freely usable.

NOTE: In some systems, you'll need to replace `python` with `python3`. This
goes for all the commands (for here, I'll use `python`).


## USING `weatherData.py`
This works the same as `sensorData.py`, only not as a root user:

`python weatherData.py [DATA FILE].txt`

To reduce confustion, name weather data files with the following format:

`weather_[ABBREVIATION OF MONTH WHEN DATA COLLECTION STARTS][# OF DAY WHEN DATA COLLECTION STARTS].txt`

It also doesn't take a port number.

The unique feature of this program is its use of `requests`, a Python library 
that allow the program to fetch information from websites.

`requests` is used to get Charlottesville humidity, temperature, and 
precipitation values using WTTR, a "console-oriented weather forecast service" 
that allows users to collect meteorological data from anywhere in the world. 
The point of collecting this data for our purposes is to compare our measured 
values from the sensor with those outside in the city to see how the measured 
values fluctuate with the outside environment. This data is written to the 
second data file.


## USING `NEW_READER.py`
This new program takes an arbitrary number of data files in any order and plots 
them all on graphs. It is used as follows:

`python NEW_READER.py [DATA FILE 1].txt [DATA FILE 2].txt [DATA FILE 3].txt ...`

The reasons it takes an arbitrary number of files are:
1. There are multiple sensors running at the same time.
2. There is data from multiple time periods.

`NEW_READER.py` can plot data from just one sensor, multiple sensors, multple 
sensors with a file of weather data, multiple files of weather data, or any 
combination of sensor and weather data, across the same or differing time 
periods for each. The files can be passed in any order to prevent mistakes in 
plotting. As long as at least one file is passed, the program will plot it.

In addition, `NEW_READER.py` will also display the mean and standard
deviation of the humidities and temperatures from each dataframe to the
screen. There is an option that allows the user to choose whether they want
to display these statistics every 10 seconds (used mainly for updating data
files). The program also calculates and displays the absolute humidity values
based on the relative humidity and temperature readings.

The nice thing about `NEW_READER.py` is that it can display the data as it 
is being taken. This was implemented 6/8/2024 -- 6/10/2024 and allows the user
to see the data as it is updating and make decisions about where to go from
there. Further updates have allowed the user to be able to change the ranges
of the humidity and temperature graphs, choose whether they want to use a
specific date range for both plotting and statistics (and if so, denote the
start and/or end dates of the range), choose whether they want the statistics
to update, and choose various attributes about the plots. It makes using the
program very user-friendly.

Lastly, the user can type the following to get a list of tips for using the
program:

`python NEW_READER.py HELP`


## SUMMARY
The following commands, in order, are how to run and plot the data from the 
sensor:
1. `arduino --upload --port [PORT] aht10.ino`
2. `sudo su`
3. `python sensorData.py [INTEGER] [DATA FILE].txt`
4. `python weatherData.py [DATA FILE].txt`     

If we are using a Geekom PC, switch command #1 with the following commands:

1. `./[PATH IF NEEDED]/arduino-cli compile -b [BOARD NAME] [FILE PATH]/[FILE NAME]`
2. `./[PATH IF NEEDED]/arduino-cli upload -p [PORT] [FILE PATH]/[FILE NAME]`

or

`./[PATH IF NEEDED]/arduino-cli upload [FILE PATH]/[FILE NAME] -p [PORT] -b [BOARD NAME]`

To plot the data, using the following command:

`python NEW_READER.py [DATA FILE 1].txt [DATA FILE 2].txt [DATA FILE 3].txt ...`

NOTE: You can also specify a file path to a data file in another directory. And 
if the files all have a common beginning, you can use the `*` method to use them 
all with the script. As an example:

`python NEW_READER.py data_runs/run000001*`

This example assumes you are in the directory where `NEW_READER.py` is 
located and that the directory `data_runs/` is a subdirectory of your current 
working directory.


## TROUBLESHOOTING
### Check Permissions on Files and Programs
If a program isn't working, check the permissions on it using the following 
command:

`ls -l(h)`

(The `-h` flag stands for "human-readable format" and displays everything in a
more comprehensible format.)

If the desired file has root permissions, use the following command to change it
 to user permissions:

`sudo chown [USER]:[USER] [FILE NAME]`

`[USER]` is the name of the user found if you execute `pwd` (or when looking at 
the current directory) while `[FILE NAME]` is the file for which you want to 
change permissions. This command works if you are not a root user at the moment,
 and it will ask you to enter your sudo password. If you are a root user at the 
moment, omit `sudo` from the command.

You can change the read (r), write (w) and execute (x) permissions on a file by
using the following command:

`sudo chmod +(-)r(w)(x) [FILE(DIRECTORY)]`

You can add "+" (or remove "-") any combination of r, w, and x on a file or
directory. If you're already acting as a root user, you can omit `sudo`.

### Humidity and Temperature Values Switch
Sometimes, when executing `sensorData.py`, the timing gets off with the sensor 
and the values that are listed as humidities are actually temperatures and vice 
versa. To fix this, simply use `Ctrl-C` or `Ctrl-Z` (the latter is slightly
nicer to the program) and rerun the command with different data files.

NOTE: This bug was fixed by editing `aht10.ino` to send `sensorData.py` a 
counter. `sensorData.py` will then determine whether that counter is odd or 
even, and depending on which it is, it will write temperature or humidity 
measurements to the data file.

### Can't Push Files to GitHub
Sometimes, changes to your files and those that appear on GitHub become out of 
sync. This can happen if you accidentally commit something but forget to push it
 or if you modify your files in GitHub when you usually do it on the command 
line. To check the status of Git, execute the following command:

`git status`

This command will let you know whether your changes are up to date. If they 
aren't, you'll be ahead or behind by a certain number of commits. To fix this, 
execute the following command:

`git reset --soft origin/[BRANCH NAME]`

You can determine the branch name by running `git branch --show-current`.

### Removing Files from GitHub but not Local Directory

Use the following commands:

`git rm --cached [FILES]`  
`git commit -m "[MESSAGE]"`  
`git push`

This series of commands is similar to adding files to GitHub, but instead 
of just saying `rm` in this case, you must include `--cached`; otherwise,
 your files will be deleted from your local directory, too.

### Getting Changes Pushed from One Local Repository to Another
If your GitHub repository is connected to several different local repositories
on different machines and you make changes to your files on one repository but
not another, you will first need to push your changes to GitHub from your
current local repository (call it R1 for our purposes) and then pull the
changes pushed to GitHub to the other local repository (call it R2).

In R2, run the following command:

`git pull`

You may be prompted to enter your sudo password. If so, enter it. The changes
you made in R1 will then be given to R2. It is important to note that if you
made changes to the files in R2, they will be overwritten by the changes you
made in R1. Either copy the files if you want to record those changes
somewhere or accept defeat.

If you've made changes to the files in R2, you may run into a situation where 
fetching and pulling will tell you that you're either already up to date on R2 
or that you must commit or stash your changes before pulling. Even if you want 
to overwrite the changes you made in R2, you won't be able to. So, use this 
series of commands:

`git fetch --all`  
`git branch backup-master`  
`git reset --hard origin/master`

`git fetch` downloads the latest files from the remote repository without trying
 to merge or rebase; `git reset` resets the master branch to what you just 
fetched, while the `--hard` option changes your files in the working tree to 
match the files in `origin/master`.

[Source](https://stackoverflow.com/questions/1125968/how-do-i-force-git-pull-to-overwrite-local-files)

7/15/2025 UPDATE: In the event that several branches are in use, use the following
commands:

`git checkout [NEW BRANCH]` (switch to new branch)  
`git push -u origin [NEW BRANCH]` (push to new branch)

For some general commands, use:

`git checkout -b [NEW BRANCH]` (create and switch to new branch)
`git branch -a` (see all branches)

### Initializing Repository and Connecting to GitHub
Check to see if git is installed on the machine you want to connect to
GitHub by running the following command:

`git --version`

If git is installed, it will return the version of git on the machine. If
not, install git using either a Debian/Ubuntu system installation (top) or
 a Fedora/CentOS system installation (bottom):

`sudo apt-get update` + `sudo apt-get install git-all`
`sudo dnf(yum) update` + `sudo dnf(yum) install git-all`

Create a directory where you want to initialize git and type the following
command:

`git init`

You must create a new SSH key and connect it to your GitHub repository. To
do this, run the following command:

`ssh-keygen -t ed25519 -C [GITHUB EMAIL]`

where your GitHub email is the email to which your GitHub is linked. The screen
will prompt you to enter a file in which to save the key, but hitting enter will
 save the key to the default location.

Next, start the ssh-agent in the background with the following command:

`eval "$(ssh-agent -s)"`

Then, add your key to the ssh-agent with the following command:

`ssh-add ~/.ssh/id_ed25519`

Once this is done, copy the SSH key to the clipboard. It can be found by
using the following command:

`cat ~/.ssh/id_ed25519.pub`

Then, go to your GitHub account and click "Settings." Under the "Access"
section of the sidebar, click "SSH and GPG Keys." Click "New SSH Key" and
title the key to fit your needs. After selecting the key type, paste the key
 to the field and click "Add SSH Key." The key should be ready for use now.

If you have a repository on GitHub already, you can clone that repository to
 your local directory with the following command:

`git clone git@github.com:[USERNAME]/[REPOSITORY].git`

where your username is your GitHub username and the repository is the
repository you want to clone.

If you don't have a repository on GitHub already, you can use the series of
 commands in the directory where you have the code you want to track with
git:

`git add [FILES]`  
`git commit -m "[MESSAGE"]`  
Click "New Repository" in GitHub, name it the same as your directory, and
click "Create Repository"  
`git remote add origin git@github.com:[USERNAME]/[REPOSITORY].git`  
`git push -u origin master`

The repositories should be linked now. Ideally, you'll create a `README.md` file
 and a `.gitignore.txt` file.

[Source 1](https://kbroman.org/github_tutorial/pages/init.html) 
[Source 2](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) 
[Source 3](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account)

### Merging Data into One File
To merge different datasets into one file, simply use the following command:

`cat [DATA FILE 1].txt [DATA FILE 2].txt ... > [MERGED DATA FILE].txt`

### `NEW_READER.py` Taking Long Time to Load
Just be patient. It's working, I promise.


## ACKNOWLEDGEMENTS
Code written by Christian Guinto-Brody for Professor Chris Neu's research group.