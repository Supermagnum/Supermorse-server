# Supermorse Modified Mumble Server

This repository contains a modified version of the Mumble server (Murmur) specifically designed for the Supermorse application. The server includes special features for amateur radio simulation, including HF band channels and realistic propagation simulation.

## Features

### HF Band Channels
- Channels corresponding to amateur radio frequencies (160m, 80m, 60m, 40m, 30m, 20m, 17m, 15m, 10m, 6m)
- Channel descriptions with frequency ranges
- Channel links to simulate propagation overlap between bands

### Advanced Propagation Simulation
- **Realistic HF Propagation Model**: Based on solar conditions, geomagnetic activity, and seasonal variations
- **Solar Flux Index (SFI)**: Simulates the effect of solar activity on HF propagation
- **K-index**: Simulates the effect of geomagnetic activity on HF propagation
- **Seasonal Variations**: Simulates the effect of seasons on HF propagation
- **Day/Night Path Calculation**: Simulates the effect of daylight on HF propagation
- **Maximum Usable Frequency (MUF)**: Calculates the highest frequency that will be reflected by the ionosphere
- **Lowest Usable Frequency (LUF)**: Calculates the lowest frequency that will penetrate the D-layer absorption
- **Signal Strength Calculation**: Based on distance, frequency, solar conditions, and day/night path
- **Audio Quality Simulation**: Applies fading and noise based on signal strength
- **Real-time Propagation Data**: Integration with hf.dxview.org for band-specific propagation data
- **Real-time Solar Weather Data**: Integration with swpc.noaa.gov for solar flux and geomagnetic activity data

### Maidenhead Grid Locator Support
- User metadata field for grid locator
- Distance and bearing calculations between grid locators
- Band recommendations based on distance and propagation conditions

### Custom User Experience
- Preferred HF band selection
- Band recommendations based on current propagation conditions
- Signal strength indicators
- Propagation condition updates

## Building the Server

### Prerequisites

- CMake 3.15 or higher
- Qt 5.12 or higher
- C++ compiler with C++14 support (GCC, Clang, MSVC)
- SQLite development libraries

### Building on Linux/macOS

```bash
# Clone the repository
git clone https://github.com/Supermagnum/Supermorse-server.git
cd supermorse-mumble

# Build the server
./build.sh
```

### Building on Windows

```batch
# Clone the repository
git clone https://github.com/Supermagnum/Supermorse-server.git
cd supermorse-mumble

# Build the server
build.bat
```

## Configuration

The server is configured using the `mumble-server.ini` file in the `config` directory. This file includes settings for:

### Server Settings
- Server name and welcome message
- Maximum number of users
- Default channel
- Server port and host
- Database configuration

### HF Band Simulation Settings
The `[hf_propagation]` section contains settings for the HF band simulation:

```ini
[hf_propagation]
; Enable or disable HF band simulation
enabled=true

; Solar Flux Index (SFI) - Measures solar activity
; Range: 60-300, higher values mean better propagation on higher bands
solar_flux_index=120

; K-index - Measures geomagnetic activity
; Range: 0-9, lower values mean better propagation
k_index=3

; Auto-detect season based on current date
auto_season=true

; Manual season setting (only used if auto_season=false)
; 0=Winter, 1=Spring, 2=Summer, 3=Fall
season=0

; Update interval for propagation conditions in minutes
update_interval=15
```

### Channel Configuration
The server includes predefined channels for HF bands:

```ini
[channels]
; Root channel
0=Root

; HF Band channels
1=160m
2=80m
3=60m
4=40m
5=30m
6=20m
7=17m
8=15m
9=10m
10=6m
```

### Channel Links
Channel links are used to simulate propagation overlap between bands:

```ini
[channel_links]
; Link channels that might have propagation overlap
1=2
2=1,3
3=2,4
4=3,5
5=4,6
6=5,7
7=6,8
8=7,9
9=8,10
10=9
```

### Custom Metadata Fields
The server supports custom metadata fields for grid locators and preferred bands:

```ini
[metadata_fields]
; Maidenhead grid locator
maidenheadgrid=text

; Preferred HF band
preferredhfband=select:160,80,60,40,30,20,17,15,10,6
```

## Running the Server

After building, the server executable can be found in the `build/bin` directory. To run the server:

```bash
# Linux/macOS
./build/bin/murmur -c config/mumble-server.ini

# Windows
build\bin\Release\murmur.exe -c config\mumble-server.ini
```

Command-line options:
- `-c, --config <file>`: Specify the configuration file to use
- `-d, --database <file>`: Specify the database file to use

## Testing the Server Features

To verify that all features of the modified Mumble server are working correctly, you can use the following testing procedures with the SuperMorse application.

### Prerequisites for Testing

1. **Build and run the server** using the instructions above
2. **Prepare the SuperMorse app**:
   - Ensure you have the latest version of the SuperMorse app from https://github.com/Supermagnum/supermorse-app
   - Create a user account with 100% mastery of the International Morse alphabet, numbers, and prosigns
   - If testing only, you can temporarily modify the `connect()` method in `murmur.js` to bypass the mastery check

### Basic Connection Testing

1. Launch the SuperMorse app with `npm run dev`
2. Navigate to the Murmur communication interface
3. Enter your server address (e.g., `localhost` or the IP address with port)
4. Click the "Connect" button
5. Verify that:
   - The connection status changes to "Connected"
   - The server status indicator turns green
   - The channels dropdown populates with HF bands

### Channel/Band Switching Testing

1. After connecting, use the channel/band dropdown selector
2. Select different HF bands (e.g., 20m, 40m, 80m)
3. Verify that:
   - The current band display updates accordingly
   - The propagation quality indicator changes based on the selected band
   - The system message confirms you've joined the new channel

### Messaging Testing

1. Type a Morse message in the input field
2. Send the message using the send button or Enter key
3. Verify that:
   - The message appears in your message display
   - The message includes your callsign/username and timestamp
   - If another client is connected, verify they receive the message

### User/Station Tracking Testing

1. Connect with multiple clients if possible
2. Verify that:
   - The stations list populates with connected users
   - Each station shows the correct callsign and channel/band
   - Selecting a station attempts to join their channel
   - Stations update when users change channels

### Propagation Simulation Testing

1. Switch between different HF bands
2. Observe the propagation quality indicator
3. Test at different times of day to see how the simulated propagation changes
4. Verify the propagation conditions affect:
   - The propagation quality indicator level (1-5)
   - The ability to communicate between stations at different distances
   - The recommended bands for your current conditions

### Advanced Testing with Multiple Clients

For more thorough testing, set up multiple clients:

1. Run the SuperMorse app on multiple computers connected to the same server
2. Configure each client with different Maidenhead grid locators (in settings)
3. Test how distance affects:
   - Propagation between different bands
   - Signal quality between stations
   - Band recommendations
4. Test in different propagation conditions by adjusting the server's `solar_flux_index` and `k_index` values in the configuration file

### Testing Propagation Configuration

To test how different propagation conditions affect the simulation:

1. Stop the server
2. Edit the `config/mumble-server.ini` file
3. Modify the `[hf_propagation]` section values:
   - Set `solar_flux_index` to different values (60-300)
   - Set `k_index` to different values (0-9)
4. Restart the server and observe the changes in propagation

### Manual API Testing

For developers, you can test the IPC methods directly from the SuperMorse app's DevTools console (when running in dev mode):

```javascript
// Test connection
window.electronAPI.connectMumble('localhost', {
  username: 'TestUser',
  channelName: '20m'
}).then(console.log);

// Test sending a message
window.electronAPI.sendMumbleMessage('CQ CQ DE TEST').then(console.log);

// Test getting channels
window.electronAPI.getMumbleChannels().then(console.log);

// Test getting users
window.electronAPI.getMumbleUsers().then(console.log);
```

### Troubleshooting

If you encounter connection issues:

1. Check the server is running and accessible (try pinging the server)
2. Verify the server port is open and not blocked by a firewall
3. Check certificates if using secure connection
4. Look for error messages in:
   - The server console output
   - The SuperMorse app's DevTools console (press F12 in dev mode)
5. Verify your user account has the required Morse code mastery levels
6. Check the Maidenhead grid locator in your settings is valid

By following this testing plan, you'll be able to verify all the features of the modified Mumble server integration in the SuperMorse app.

## Project Structure

### Modular Architecture

The server uses a modular architecture to separate concerns and improve maintainability:

- **Module Interface**: The `IServerModule` interface defines the contract for all server modules
- **Module Manager**: The `ModuleManager` class handles registration and lifecycle of modules
- **Modules Directory**: All module-related files are organized in the `src/murmur/modules` directory:
  - `IServerModule.h` - Base interface for all modules
  - `ModuleManager.h/cpp` - Module registration and management
  - `UserDataModule.h/cpp` - User authentication and data management
  - `PropagationModule.h/cpp` - HF propagation simulation
  - `HFBandSimulation.h/cpp` - Core simulation engine

This organization makes it easy to add new modules or modify existing ones without affecting other parts of the system.

## How the HF Band Simulation Works

### Propagation Model

The HF band simulation uses a realistic propagation model based on:

1. **Solar Flux Index (SFI)**: Measures solar activity, which affects the ionization of the F-layer. Higher SFI values improve propagation on higher frequencies.
   - 60-80: Poor conditions (lower bands only)
   - 80-110: Fair conditions (mid-range bands)
   - 110-150: Good conditions (higher bands during day)
   - 150-300: Excellent conditions (all bands, 10m open during day)

2. **K-index**: Measures geomagnetic activity, which can disrupt the ionosphere. Lower K-index values mean better propagation.
   - 0-2: Quiet conditions (excellent propagation)
   - 3-4: Unsettled conditions (good propagation)
   - 5-6: Minor storm (degraded propagation on polar paths)
   - 7-9: Major storm (severely degraded propagation)

3. **Seasonal Variations**: Different seasons affect propagation due to changes in ionization and day length.
   - Winter: Better nighttime propagation on lower bands
   - Summer: Better daytime propagation on higher bands
   - Spring/Fall: Balanced propagation conditions

4. **Day/Night Path**: The percentage of the path between two stations that is in daylight affects propagation.
   - Daytime: Higher bands (10m-20m) work better
   - Nighttime: Lower bands (40m-160m) work better
   - Gray line: Enhanced propagation during sunrise/sunset

5. **Distance**: The distance between stations affects which bands work best.
   - Short distances (<500 km): Higher bands during day, lower bands at night
   - Medium distances (500-2000 km): Mid-range bands (20m-40m)
   - Long distances (>2000 km): Lower bands at night, higher bands during day with good conditions

### Signal Strength Calculation

Signal strength is calculated based on:
- Distance between grid locators
- Frequency (band)
- Solar flux index
- K-index
- Day/night path percentage
- Seasonal factors

The calculated signal strength affects:
- Audio quality (fading and noise)
- Whether communication is possible between users
- Band recommendations

### Band Recommendations

The server provides band recommendations based on:
- Current propagation conditions
- Distance between stations
- Time of day
- Season

Users receive recommendations when they connect and when propagation conditions change.

## Integration with Supermorse

This modified Mumble server is designed to work with the Supermorse application. The Supermorse application can start and stop this server programmatically and uses it for voice communication and HF simulation.

## For future implementanion
When the user has spent xx hours doing simulated Morse contacts, unlock SSB ”channels“ per this bandplan.
Unsure if it's possible to add a frequency dial in the GUI,and if a used servers hardware can handle the load.
https://github.com/Supermagnum/Supermorse-server/blob/main/Bandplans/band_segments.csv

Also read:
https://github.com/Supermagnum/Supermorse-server/blob/main/future-plans.md

## License

This project is based on Mumble, which is licensed under the BSD license. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- The Mumble project for the original server code
- The Supermorse project for the amateur radio simulation features
