## RoadRunner 🥇⭐- Get the best DSA roadmap tailored to your needs!
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)<br>
![Build](https://img.shields.io/badge/build-passing-brightgreen)<br>

### A personalized DSA roadmap generator designed to take you to the sky! 💪✨

>Stacks, Dynamic Programming, or Tries??? Still stuck deciding which topic to study next? Presenting **_RoadRunner_**, your all-in-one personalized DSA roadmap generator guaranteed to get you the best DSA journey possible. Implemented entirely in C++, RoadRunner uses various algorithmic techniques like _Greedy, Dynamic Programming, File Handling and Tries_ to the best of its benefit.
<hr>

## About📝⭐<br>
Given that hundreds of thousands of CS grads enter the job market every year, DSA being the core skill tested in the interview process, its importance is undeniable. However the large number of topics need to be studied most optimally for striking the perfect balance between DSA expertise and other skill acquisition. Here's where **_RoadRunner_** comes to your rescue. Just choose the kind of roadmap you'd like to generate and it'll have it ready for you within seconds! 📔

## Features 📖✨<br>
✔️ An all-in-one personalized DSA roadmap generator.<br>
✔️  Define your own learning journey!<br>
✔️ Learn a topic from Scratch to Advanced!<br>
✔️ Have time constraints followed as well.<br>
✔️ Use your present skillset to the best of your benefit.<br>
✔️ Auto-typo-correction enabled.<br>
✔️ A simple menu based UI for easy interaction.<br>
✔️ A colorful UX for a more engaging experience.<br>

## Preview 👀<br>
![image](https://github.com/user-attachments/assets/3c766e10-22b8-488c-bdc3-7d2757759583)
<hr>

## Getting Started ⚡

### Prerequisites 📌
✅ A C++ compatible compiler<br>
✅ Any OS : Windows/ Mac/ Linux<br>
<br>

### Build and Run 🚀

```bash
# Clone the repository
git clone https://github.com/Vaidehi-05/RoadRunner.git

# Navigate to the project folder
cd RoadRunner

# Compile
g++ CompleteCode.cpp -o RoadRunner

# Run
./RoadRunner

```

###  How it works 🛠️
1️⃣ Run RoadRunner and select your preferred roadmap specs.<br>
2️⃣ Enter your current skillset.<br>
3️⃣ All the topics are stored internally in a txt file, through which graphs are internally created through file parsing.<br>
4️⃣ Just add in your preferences.<br>
5️⃣ Enjoy real productivity with your custom roadmap!<br>

### File Structure 📖
💠FORMAT FOLLOWED:<br>
➡ Num_of_nodes:<br>
➡ Name:<br>
➡ ConnectedNodes:<br>
➡ EdgeStart:<br>
➡ EdgeEnd:<br>
➡ EdgeWeight:<br>
➡ EdgeTimeNeeded:<br>
💠 The graphs formed through this file, can be further visualized using our custom graph visualizer. 🌻

### Algorithms Implemented 🥇

| Algorithm                                 | Purpose                                                           |
| ------------------------------------------| ------------------------------------------------------------------|
| Trie                                      | Prefix Search and Matching  (Smart Search)                        |
| Dynamic Programming (Levenshtein Distance)| Auto-correct and validate user input, fixing typos (Smart Search) |
| Modified DFS                              | Priority Based Graph Traversal                                    |
| Modified Dijkstra's Algorithm            | Most Optimal Path from Current to Aimed Skillset                  |
| Greedy (0/1 Knapsack Implementation)      | Pick/ Not pick depending on constraints                           |
<hr>

## Project Structure 📁

| File               | Purpose                                         |
| -------------------| ------------------------------------------------|
| `CompleteCode.cpp` | Core logic and menu system                      |
| `GraphDetails.txt` | File storing all Topic Graphs                   |
| `abbreviation.h`   | Header file storing popular topic abbreviations |
| `graph_visual.html`| Visualization and Editing of Graphs with interaction |
| `README.md`        | Project Documentation                           |
<hr>

>Happy Studying! 📖🥇
