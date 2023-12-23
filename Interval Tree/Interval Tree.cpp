#include <iostream>
#include <cstring>

using namespace std;
struct Interval {
    int low, high; // store the low and high of the inerval [low, high]
    Interval(){}
    Interval(int low, int high):low(low), high(high){}
    
};
class Node {
private:
public:
    Interval interval; // each node has an interval [low, high]
    int max; // max is the max(node->left.max, node->right.max, node->interval.high)
    Node* left;
    Node* right;
    Node(): left(nullptr), right(nullptr){}
};
class IntervalTree {
public:
    Node* root;
    IntervalTree():root(nullptr){}
    Node* getRoot() { return root; }


    // used to update the new node that will be inserted in the tree
    Node* updateData(Interval interval) {
        Node* newnode = new Node(); // set left and right to nullptr
        newnode->max = interval.high; // make max to be the high of the interval
        newnode->interval = interval; // assign the inerval to the interval in node
        return newnode;
    }
    
    
    int postOrder(Node* root) {
        if (root->left == nullptr or root->right == nullptr) { // if it is a leaf node then return its max (base case)
            return root->max;
        }

        int leftMax = postOrder(root->left);  // get the left max
        int rightMax = postOrder(root->right);  // get the right max

        // check that the root of left and right max to update the max of the current root
        if (root->interval.high < leftMax and leftMax > rightMax) {
            root->max = leftMax;
        }
        else {
            root->max = rightMax;
        }

        return root->max;  // return max of the root to other calls...
    }
    
    
    // after inserting the we need to update all the max in the tree by using postorder traversal
    void updateMax(Node* root) {
        postOrder(root);
    }


    void insert(Node* current, Interval interval) {
        if (current == nullptr) { // if there is no intervals in the tree or first interval to be inserted
            root = updateData(interval);
        }
        else {
            // traverse is used to traverse the tree to get to the correct position to insert the node
            // prev is used to save the value of the previous node of the traverse to insert the node to left or right
            Node* traverse = current, *prev = nullptr;
            while (traverse) {
                // if the low of the current low is bigger than the new interval then go left
                if (traverse->interval.low > interval.low) {
                    prev = traverse;
                    traverse = traverse->left;
                }
                // if the low of the current low is smaller than the new interval then go rught
                else if (traverse->interval.low < interval.low) {
                    prev = traverse;
                    traverse = traverse->right;
                }
            }
            // check if the low in the interval is smaller then it will be inserted in the left
            if (prev->interval.low > interval.low) {
                prev->left = updateData(interval);
            }
            // and if it is bigger it will be inserted in the right
            else {
                prev->right = updateData(interval);
            }

            updateMax(root); // update the max of each node in the tree after a new interval is inserted
            
        }
    }
    
    void printIntervalTree(Node* root) {
        if (!root) return;
        printIntervalTree(root->left);
        cout << "[ " << root->interval.low << ", " << root->interval.high << " ] ---> MAX is " << root->max << endl;
        printIntervalTree(root->right);
    }

    bool overlapping(Interval currentInterval, Interval query) {
        return (currentInterval.high >= query.low)
            and (currentInterval.low <= query.high);
    }

    Node* search(Node* root, Interval query) {
        if (root == nullptr) return nullptr;

        if (overlapping(root->interval, query)) {
            return root;
        }

        if (root->left != nullptr and query.low <= root->left->max) {
            return search(root->left, query);
        }

        return search(root->right, query);
    }
};
int main(){
    IntervalTree tree;

    // Test case: Insert intervals into the tree
    tree.insert(tree.getRoot(), Interval(17, 19));
    tree.insert(tree.getRoot(), Interval(5, 11));
    tree.insert(tree.getRoot(), Interval(22, 23));
    tree.insert(tree.getRoot(), Interval(4, 8));
    tree.insert(tree.getRoot(), Interval(15, 18));
    tree.insert(tree.getRoot(), Interval(7, 10));

    tree.printIntervalTree(tree.getRoot()); // traverse through the interval tree

    Node* ans = tree.search(tree.getRoot(), Interval(6, 9));

    if (ans != nullptr) {
        cout << "Found Interval: [" << ans->interval.low << ", " << ans->interval.high << "]" << endl;
    }
    else cout << "NOT FOUND\n";

    return 0;
    
}
