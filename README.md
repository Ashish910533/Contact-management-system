📇 Contact Management System
A cross-platform Contact Management System built in C using GTK+ for GUI, integrated with Dropbox API for cloud storage. This system also connects to a web-based interface built with React, Firebase Authentication, and Node.js, allowing easy CRUD operations on contacts and access from anywhere.

✨ Features
Add, view, search, and delete contacts with:
📞 Name
📱 Phone Number
🏫 Department (via dropdown)
📅 Current Date

Dropbox integration to:
☁️ Upload contact list to the cloud
🔄 Enable remote backup & sharing

Web-based interface (React + Firebase):
🔐 Secure login
📋 Easy view and update contacts online
⚙️ Technologies Used
🔧 Backend / Core:

C Programming
GTK+ 3 (Graphical User Interface)
Dropbox C SDK (for API integration)

🌐 Web Interface:
React.js
Firebase Authentication
Node.js + Express.js (for API server)
Dropbox JS SDK

📦 Dependencies & Installation
For C GTK+ Application:
Ensure the following packages are installed:

bash
Copy code
sudo apt update
sudo apt install libgtk-3-dev libcurl4-openssl-dev libjson-glib-dev
GTK+ 3: For GUI

libcurl: For making Dropbox API calls

libjson-glib: To handle JSON data from Dropbox API

To compile:

bash
Copy code
gcc main.c -o contact_manager `pkg-config --cflags --libs gtk+-3.0 json-glib-1.0` -lcurl
🌐 Setting Up the Web Interface (Optional)
Only needed if you're using the React web UI

Install Node.js & npm:

bash
Copy code
sudo apt install nodejs npm
Clone the frontend repo (if separate):

bash
Copy code
git clone https://github.com/yourusername/contact-manager-web
cd contact-manager-web
Install dependencies:

bash
Copy code
npm install
Start the React app:

bash
Copy code
npm start
🔐 Firebase Setup (Web Login)
Go to Firebase Console

Create a project and enable Email/Password Authentication

Copy Firebase config into your React app's firebaseConfig.js

🐞 Challenges Faced
Dropbox Integration in C:
Managing Dropbox OAuth2 flow and using libcurl for HTTP requests was tricky due to limited native SDK support in C.

Parsing JSON in C:
Using json-glib required understanding of memory management and object traversal.

GTK GUI Layout:
Designing a responsive and clean interface in GTK took time, especially aligning widgets and managing events.

React & Firebase Sync:
Ensuring consistency between C-stored data and Firebase-stored contact data required thoughtful architecture.

💡 Future Improvements
Add edit/update functionality in C GUI
Store contacts in a database (e.g., SQLite) instead of text files
Upload/download contact files automatically on Dropbox
Bi-directional sync between local and web versions

Export to CSV or PDF
🤝 Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you'd like to change.

📄 License
This project is licensed under the MIT License.

for further reference or any query Email at : Ashish9105335275@gmail.com
