# Introduction

**CustomDoorIcons** is a program designed to simplify and centralize the process of customizing the names, descriptions, and icons of multiple in-game objects that have associated text. No external websites are everything is done within the app!

This project is inspired by [html-image-to-scpsl-image](https://github.com/dooory/html-image-to-scpsl-image).

# Showcase

## Windows

![Windows Interface](https://github.com/user-attachments/assets/9006009a-f5af-4de3-8d15-63f1d2241f37)

## Linux Mint & Ubuntu

![Linux Interface](https://github.com/user-attachments/assets/413ec799-0302-4bd2-ae59-03ec504eea29)

# Tab Guide

### **Door Settings**

Here you can customize the text displayed on doors during specific actions.

- **Label:** Each text field has a label above it indicating the action it corresponds to.
- **Size:** Next to the text field, specify the size of the text as it appears in-game.
- **Color Picker:** Select a color for your text that will be displayed in-game.
- **Ignore Size:** Enable this checkbox to use the default in-game font size. It is recommended to enable this if you're not using emojis.
- **Select Image:** Click this button to choose an image that will be displayed during a specific door state.

**Saving Changes:**
1. Click the **Save** button.
2. A dialog will appear prompting you to select a destination path to save the `Doors.txt` file containing your modifications.
3. Save it directly to your game directory, typically located at:
steam/steamapps/common/SCP Secret Laboratory/Translations/en


**Note:** If you leave the text fields empty, the application will use the default values for those fields.

### **Team Names**

Here you can customize the text for SCPs, human classes, and other team names.

- **Label:** Each text field has a label above it indicating the name of the class or character.
- **Text Field:** Enter your desired name or leave it empty to use the default.

**Saving Changes:**
1. Click the **Save** button.
2. A dialog will appear prompting you to select a destination path to save the `Classes_Names.txt` file containing your modifications.
3. Save it directly to your game directory, typically located at:
steam/steamapps/common/SCP Secret Laboratory/Translations/en


**Note:** If you leave the text fields empty, the application will use the default values for those fields.

### **Intercom**

Here you can customize the texts displayed on the intercom during specific actions.

- **Label:** Each text field represents a specific action, as described by the label above it.
- **Multi-Line Text:** Press Enter to add multiple lines; each line will be displayed sequentially on the intercom.
- **Timer:** Some fields include `{TIME_LEFT}`, indicating where the countdown will appear by default. To add a timer, simply check the **Add Timer** checkbox.

**Saving Changes:**
1. Click the **Save** button.
2. A dialog will appear prompting you to select a destination path to save the `Intercom.txt` file containing your modifications.
3. Save it directly to your game directory, typically located at:
steam/steamapps/common/SCP Secret Laboratory/Translations/en


**Note:** If you leave the text fields empty, the application will use the default values for those fields.

### **Items**

Here you can customize the names and descriptions of items (where available).

- **Name:** Enter the desired name for the item or leave it empty to use the default.
- **Description:** Enter the desired description for the item or leave it empty to use the default.

**Saving Changes:**
1. Click the **Save** button.
2. A dialog will appear prompting you to select a destination path to save the `Items.txt` file containing your modifications.
3. Save it directly to your game directory, typically located at:
steam/steamapps/common/SCP Secret Laboratory/Translations/en


**Note:** If you leave the text fields empty, the application will use the default values for those fields.

# Troubleshooting - Linux

### **The App Won't Open After Double-Clicking on it**

**For Linux Mint:**
1. Right-click the executable file.
2. Select **Properties**.
3. Navigate to the **Permissions** tab.
4. Check the **Allow executing file as program** box.

**For Other Distributions:**
1. Open your terminal.
2. Navigate to the directory containing the executable.
3. Run the following commands:

sudo chmod +x CustomDoorIconsLinux
./CustomDoorIconsLinux

### **Tab Names Disappear When Clicked / Typed Text Isn't Showing Up / Buttons Do Not Have Text**
Cause: This issue typically occurs when using a light theme. CustomDoorIcons isn't fully compatible with light themes because the text color (white) blends into the background, making it appear invisible.

Solution: Change your system theme to a dark theme. This will ensure that all text elements are visible. Future updates may address compatibility with light themes.

### **Fonts Aren't Appearing in Team Names/Door Settings**
Cause: The fonts used in the Team Names and Door Settings tabs are purely visual and are currently only available on Windows. This is because Windows provides a library to load hard-coded fonts, which isn't natively available on Linux.
