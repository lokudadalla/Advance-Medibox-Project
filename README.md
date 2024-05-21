# Advance-Medibox-Project

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
    <h1>Medibox Project Overview</h1>
    <p>The Medibox project, developed as part of the EN2853: Embedded Systems and Applications course, leverages the ESP32 microcontroller to create an intelligent medication management system. This system ensures that users are reminded to take their medicine on time while maintaining optimal storage conditions for light-sensitive medications. The project combines two major programming assignments, each contributing distinct functionalities to the Medibox.</p>

  <h2>Core Functionalities</h2>
    <ol>
        <li>
            <strong>Time Management:</strong> The device fetches the current time from an NTP server and displays it on an OLED screen. Users can set the time zone and three alarms, which alert users when it is time to take their medication. Alarms can be stopped with a push button and disabled when not needed.
        </li>
        <li>
            <strong>Environmental Monitoring:</strong> The Medibox monitors temperature and humidity levels, providing warnings when conditions exceed healthy limits (26°C ≤ Temperature ≤ 32°C and 60% ≤ Humidity ≤ 80%). These alerts ensure the medications are stored under optimal conditions.
        </li>
    </ol>

  <h2>Enhanced Functionalities</h2>
    <ol>
        <li>
            <strong>Light Sensitivity Management:</strong> To accommodate light-sensitive medications, two Light Dependent Resistors (LDRs) are used to monitor light intensity. The highest intensity is displayed on a Node-RED dashboard, indicating whether it is detected by the left or right LDR. A shaded sliding window, controlled by a servo motor, adjusts based on light conditions to protect the medications from excessive light.
        </li>
        <li>
            <strong>User Interface Enhancements:</strong> The Node-RED dashboard includes sliders for adjusting the minimum angle and controlling factor of the shaded window, as well as a dropdown menu for selecting specific medications. This interface allows users to tailor the Medibox settings to meet the specific requirements of different medications.
        </li>
    </ol>

   <h2>Implementation Details</h2>
    <p>The project employs the following technologies and techniques:</p>
    <ul>
        <li><strong>ESP32 Microcontroller:</strong> Central to the project, the ESP32 handles time synchronization, environmental monitoring, and controlling the servo motor for the shaded window.</li>
        <li><strong>Wokwi Simulation:</strong> Used for initial development and testing of the Medibox functionalities.</li>
          <img src="your-image-url-here" alt="Medibox Project Image" width="600">

        <li><strong>Node-RED Dashboard:</strong> Provides a user-friendly interface for monitoring and adjusting Medibox settings, enhancing the interaction and customization capabilities.</li>
        <li><strong>MQTT Protocol:</strong> Facilitates communication between the ESP32 and the Node-RED dashboard, ensuring real-time updates and control.</li>
    </ul>

   <h2>Conclusion</h2>
    <p>The Medibox project demonstrates a comprehensive approach to developing an intelligent medication management system. By integrating time management, environmental monitoring, and light sensitivity features, the Medibox ensures that users can maintain their medication schedule and store their medications under optimal conditions. This project showcases the application of microcontroller programming and IoT technologies in creating practical solutions for healthcare management.</p>
</body>
</html>
