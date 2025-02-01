# CPSboost

CPSboost is a program designed to increase the number of clicks per single press of your mouse button. It's an ideal tool for those who want to enhance their clicking efficiency in various applications or games.

---

## How It Works

When you press buttons like the left or right mouse button, the program automatically adds a specific number of additional clicks. These clicks are generated almost instantaneously, and the exact number varies based on the average value you set in the configuration file.

---

## Click Mechanics

The additional clicks are not identical each time; instead, they fluctuate around the average value you define in the config file. This randomness ensures a more natural feel to the clicks, avoiding patterns that could be flagged as unnatural by some systems.

---

## What Happens After Launch?

Once you run the program:
- A console window will open.
- A configuration file will be created automatically.
- You can modify the configuration file even while the program is running.
- The keybind for toggling the program's functionality is specified in the config file.

---

## Configuration File: `config.cfg`

Below is an example of what the `config.cfg` file looks like, along with explanations for each setting:

```ini
average_cps_left=10
average_cps_right=10
keybind=M
```

### Explanation of Each Setting:

- **`average_cps_left=10`**:  
  This sets the average number of clicks generated per press for the **left mouse button**. The actual number of clicks will vary slightly around this value to simulate natural clicking behavior.

- **`average_cps_right=10`**:  
  Similar to the left button, but this setting controls the **right mouse button**. Adjust this value if you want different behavior for the right button.

- **`keybind=M`**:  
  This is the key you press to toggle the program's functionality **PAUSE** or **UNPAUSE**. By default, it's set to **M**, but you can change it to any single character (e.g., 'F', 'Q', etc.).

---

## Manual Compilation

If you wish to compile the program manually, here’s what you need to know:

This project was developed using **Visual Studio 2022** with the latest version of **C++**. The encoding used is **Multi-Byte Character Set (MBCS)**. To compile the program, follow these steps:

1. Open the project in Visual Studio.
2. Ensure that the project settings are configured to use Multi-Byte Character Encoding.
3. Navigate to the top menu and select **Build > Build Solution** to compile the project.
4. Once compiled, the executable will be located in the output directory.

For advanced users, ensure that all dependencies are correctly linked, and the build environment is properly configured to match the project requirements.

---

```
Thank you for your attention!
```
