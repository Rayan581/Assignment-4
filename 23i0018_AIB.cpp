// Rayan Ahmed
// 23i-0018
// Assignment 4

#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <fstream>

using namespace std;

/**
 * @brief This class implements the doubly linked list, that can also be implemented as a stack or a queue.
 * @tparam type The type of data to be stored in the list.
 */
template <typename type>
class LinkedList
{
private:
    struct Node
    {
        type data;
        Node *next;
        Node *prev;

        Node()
        {
            next = nullptr;
            prev = nullptr;
        }
    };
    Node *head;
    Node *tail;

public:
    int size;
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~LinkedList()
    {
        clear();
        delete head;
        delete tail;
    }

    // Clears the list
    void clear()
    {
        while (head != nullptr)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    // Returns if the list is empty
    bool isEmpty() const { return head == nullptr; }

    // Adds a node to the list
    void add(const type &data)
    {
        Node *newNode = new Node;
        newNode->data = data;
        newNode->next = nullptr;
        newNode->prev = tail;

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    // Adds an element at a given index
    void add(const type &data, int index)
    {
        if (index < 0 || index > size)
            return;

        if (index == size)
        {
            add(data);
            return;
        }
        else if (index == 0)
        {
            Node *newNode = new Node;
            newNode->data = data;
            newNode->next = head;
            newNode->prev = nullptr;
            head->prev = newNode;
            head = newNode;
            size++;
            return;
        }
        else
        {
            Node *current = head;
            for (int i = 0; i < index - 1; i++)
                current = current->next;

            Node *newNode = new Node;
            newNode->data = data;
            newNode->next = current->next;
            newNode->prev = current;
            current->next->prev = newNode;
            current->next = newNode;
            size++;
            return;
        }
    }

    // Removes a node from the list
    type remove(int index)
    {
        if (index < 0 || size == 0 || index >= size)
            return type();

        if (index == 0)
        {
            Node *temp = head;
            head = head->next;
            type data = temp->data;
            delete temp;
            size--;

            if (size == 0)
            {
                head = nullptr;
                tail = nullptr;
            }

            return data;
        }

        if (index == size - 1)
        {
            Node *temp = tail;
            tail = tail->prev;
            tail->next = nullptr;
            type data = temp->data;
            delete temp;
            size--;

            if (size == 0)
            {
                head = nullptr;
                tail = nullptr;
            }

            return data;
        }

        Node *current = head;
        for (int i = 0; i < index - 1; i++)
            current = current->next;

        Node *temp = current->next;
        type data = temp->data;
        current->next = temp->next;
        delete temp;
        size--;

        if (size == 0)
        {
            head = nullptr;
            tail = nullptr;
        }

        return data;
    }

    // Returns the data at the specified index
    type operator[](int index) const
    {
        if (index < 0 || size == 0 || index >= size)
            return type();

        Node *current = head;
        for (int i = 0; i < index; i++)
            current = current->next;

        return current->data;
    }

    // Returns first element of list
    type first() const
    {
        if (head == nullptr)
            return type();
        return head->data;
    }

    // Returns last element of list
    type last() const
    {
        if (tail == nullptr)
            return type();
        return tail->data;
    }

    // Prints the list
    void print();
    // Convert a char list to string
    string to_string() const;
    // Add a string to a char list
    void add(const char *data);
};

// Print funtion for char
template <>
void LinkedList<char>::print()
{
    if (head == nullptr)
        return;
    Node *current = head;
    while (current != nullptr)
    {
        printw("%c", current->data);
        current = current->next;
    }
}

// Print function for LinkedList
template <>
void LinkedList<string>::print()
{
    if (head == nullptr)
        return;
    Node *current = head;
    while (current != nullptr)
    {
        printw("%s", current->data.c_str());
        printw(" ");
        current = current->next;
    }
}

// Convert char list to string
template <>
string LinkedList<char>::to_string() const
{
    string result = "";
    Node *current = head;
    while (current != nullptr)
    {
        result += current->data;
        current = current->next;
    }
    return result;
}

// Add a string to a char list
template <>
void LinkedList<char>::add(const char *data)
{
    while (*data != '\0')
    {
        add(*data);
        data++;
    }
}

class AVLTree
{
private:
    struct Node
    {
        string word;
        Node *left;
        Node *right;
        int height;

        Node(const string &w)
        {
            word = w;
            left = nullptr;
            right = nullptr;
            height = 0;
        }
    };

    Node *root;

    int height(Node *node)
    {
        return node == nullptr ? -1 : node->height;
    }

    int getBalanceFactor(Node *node)
    {
        return node == nullptr ? 0 : height(node->left) - height(node->right);
    }

    // Update the height of a node
    void updateHeight(Node *node)
    {
        if (node != nullptr)
        {
            node->height = max(height(node->left), height(node->right)) + 1;
        }
    }

    // Right rotate subtree
    Node *rotateRight(Node *node)
    {
        Node *leftChild = node->left;
        node->left = leftChild->right;
        leftChild->right = node;

        updateHeight(node);
        updateHeight(leftChild);
        return leftChild;
    }

    // Left rotate subtree
    Node *rotateLeft(Node *node)
    {
        Node *rightChild = node->right;
        node->right = rightChild->left;
        rightChild->left = node;

        updateHeight(node);
        updateHeight(rightChild);
        return rightChild;
    }

    // Right-Left rotate subtree
    Node *rotateRightLeft(Node *node)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // Left-Right rotate subtree
    Node *rotateLeftRight(Node *node)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Insert a new node into the tree
    Node *insert(Node *node, const string &word)
    {
        if (node == nullptr)
            return new Node(word);

        if (word < node->word)
            node->left = insert(node->left, word);
        else if (word > node->word)
            node->right = insert(node->right, word);
        else
            return node;

        updateHeight(node);

        int balanceFactor = getBalanceFactor(node);
        if (balanceFactor > 1)
        {
            if (word < node->left->word)
                return rotateRight(node);
            else
                return rotateLeftRight(node);
        }
        if (balanceFactor < -1)
        {
            if (word > node->right->word)
                return rotateLeft(node);
            else
                return rotateRightLeft(node);
        }

        return node;
    }

    // Find the node with minimum value
    Node *minValueNode(Node *node)
    {
        Node *current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    // Delete a node from the tree
    Node *deleteTask(Node *node, const string &word)
    {
        if (node == nullptr)
            return node;

        if (word < node->word)
            node->left = deleteTask(node->left, word);
        else if (word > node->word)
            node->right = deleteTask(node->right, word);
        else
        {
            if (node->left == nullptr || node->right == nullptr)
            {
                Node *temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }
            else
            {
                Node *temp = minValueNode(node->right);
                node->word = temp->word;
                node->right = deleteTask(node->right, temp->word);
            }
        }

        updateHeight(node);

        int balanceFactor = getBalanceFactor(node);
        if (balanceFactor > 1)
        {
            if (getBalanceFactor(node->left) >= 0)
                return rotateRight(node);
            else
                return rotateLeftRight(node);
        }
        if (balanceFactor < -1)
        {
            if (getBalanceFactor(node->right) <= 0)
                return rotateLeft(node);
            else
                return rotateRightLeft(node);
        }

        return node;
    }

    // Print the tree in inorder
    void print(Node *node)
    {
        if (node == nullptr)
            return;
        print(node->left);
        printw("%s ", node->word.c_str());
        printw("\n");
        print(node->right);
    }

    // Delete all nodes
    void deleteAll(Node *node)
    {
        if (node != nullptr)
        {
            deleteAll(node->left);
            deleteAll(node->right);
            delete node;
        }
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree()
    {
        deleteAll(root);
    }

    // Insert a new task into the tree
    void insert(const string &word)
    {
        root = insert(root, word);
    }

    // Delete a task from the tree
    void deleteTask(const string &word)
    {
        root = deleteTask(root, word);
    }

    // Print the tasks in the tree in inorder
    void print()
    {
        print(root);
        printw("\n");
    }
};

// Load the data from dictionary.txt in the AVL tree
void load_data(AVLTree &dictionary)
{
    ifstream file("dictionary.txt");
    if (!file.is_open())
    {
        printw("Error opening dictionary.txt file.\n");
        return;
    }
    string word;
    while (file >> word)
    {
        // Smallercase the word
        for (int i = 0; i < word.length(); i++)
            if (word[i] >= 'A' && word[i] <= 'Z')
                word[i] += 32;
        dictionary.insert(word);
    }

    file.close();
}

int main()
{
    initscr(); // Start ncurses mode
    cbreak();  // Disable line buffering
    noecho();  // Don't echo input to the screen

    system("stty -ixon");

    LinkedList<char> letters;
    LinkedList<string> words; // Implementing a list of list of characters representing a string
    AVLTree dictionary;
    load_data(dictionary); // Load data from the dictionary

    while (true)
    {
        char c = getch();
        if (c == 27) // ESC key
        {
            printw("\nExiting...\n");
            break;
        }
        else if (c == KEY_BACKSPACE || c == 127) // Backspace key
        {
            if (letters.isEmpty())
            {
                letters.add(words.last().c_str());
                words.remove(words.size - 1);
            }
            else
                letters.remove(letters.size - 1);
        }
        else if (c == ' ')
        {
            words.add(letters.to_string());
            letters.clear();
        }
        else if (c == 12) // CTRL + L
            printw("CTRL + L pressed...!\n");
        else if (c == 19) // CTRL + S
            printw("CTRL + S pressed...!\n");
        else
            letters.add(c);

        clear(); // Clear the screen
        words.print();
        letters.print();
    }

    getch();
    endwin(); // End ncurses mode

    return 0;
}