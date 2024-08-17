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
          <img src="https://private-user-images.githubusercontent.com/133969661/332588910-e84e7fcb-21c5-4242-9db5-15b250aaa5b6.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjM5MTAzNDEsIm5iZiI6MTcyMzkxMDA0MSwicGF0aCI6Ii8xMzM5Njk2NjEvMzMyNTg4OTEwLWU4NGU3ZmNiLTIxYzUtNDI0Mi05ZGI1LTE1YjI1MGFhYTViNi5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQwODE3JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MDgxN1QxNTU0MDFaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT0wZGQyNGQ2MzNmY2Q5OTljNzliYzM5ZGMyYWEzZTg4MjI1NTJhM2NhOGNhNjQzYjYxZjYwMWNjNjY1MTk1M2VjJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZhY3Rvcl9pZD0wJmtleV9pZD0wJnJlcG9faWQ9MCJ9.JisZLO8an4CGwSSNw_K_FmWNoicBG0oPX0h38MaX3Uk" alt="Medibox Project Image" width="600">

<li><strong>Node-RED Dashboard:</strong> Provides a user-friendly interface for monitoring and adjusting Medibox settings, enhancing the interaction and customization capabilities.

</li>
    <img src="https://private-user-images.githubusercontent.com/133969661/332588848-fca155fb-fe82-4259-9e52-081d6b64f595.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjM5MTAzNDEsIm5iZiI6MTcyMzkxMDA0MSwicGF0aCI6Ii8xMzM5Njk2NjEvMzMyNTg4ODQ4LWZjYTE1NWZiLWZlODItNDI1OS05ZTUyLTA4MWQ2YjY0ZjU5NS5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQwODE3JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MDgxN1QxNTU0MDFaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT1iYWJmZjljZTU4MjVhOTBhYTU4Mjg0ZThkYjZkNjFhNGI2MjIzMGZhZmQ1ZTRmMDRlMGMxNTg3MmUzM2Q4NjVlJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZhY3Rvcl9pZD0wJmtleV9pZD0wJnJlcG9faWQ9MCJ9.ktVDcIcAk2SOjRKUtZAKYhvNGxVtUjo_bGqMB8t2ZwE" alt="Medibox Project Image" width="600">
        <img src="https://private-user-images.githubusercontent.com/133969661/332588969-472a32a6-e1e0-4d84-8251-44be7d358487.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjM5MTAzNDEsIm5iZiI6MTcyMzkxMDA0MSwicGF0aCI6Ii8xMzM5Njk2NjEvMzMyNTg4OTY5LTQ3MmEzMmE2LWUxZTAtNGQ4NC04MjUxLTQ0YmU3ZDM1ODQ4Ny5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQwODE3JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MDgxN1QxNTU0MDFaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT02YzIzMzM4NTkwODRhNTNhMzVhOGMwYTA1NjIwNDY5OGZkNjk5YWNkMjJiZWRlZGY0ZmUzMDZlOTFkOWZlYWFiJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZhY3Rvcl9pZD0wJmtleV9pZD0wJnJlcG9faWQ9MCJ9.7pIzN4n2tw8v3WGRISA4d5b_wBnFU2gv-2zt6_xiNt0" alt="Medibox Project Image" width="600">


<li><strong>MQTT Protocol:</strong> Facilitates communication between the ESP32 and the Node-RED dashboard, ensuring real-time updates and control.</li>
        
</ul>

   <h2>Conclusion</h2>
    <p>The Medibox project demonstrates a comprehensive approach to developing an intelligent medication management system. By integrating time management, environmental monitoring, and light sensitivity features, the Medibox ensures that users can maintain their medication schedule and store their medications under optimal conditions. This project showcases the application of microcontroller programming and IoT technologies in creating practical solutions for healthcare management.</p>
</body>
</html>
