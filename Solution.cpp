#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

// Définition de la classe TreeNode représentant un nœud dans un arbre binaire de recherche
class TreeNode {
public:
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Définition de la classe BinarySearchTree représentant un arbre binaire de recherche
class BinarySearchTree {
public:
    // Méthode pour construire un ABR à partir d'un tableau trié
    TreeNode* sortedArrayToBST(vector<int>& nums, int start, int end) {
        if (start > end) return nullptr;
        int mid = start + (end - start) / 2;
        TreeNode* root = new TreeNode(nums[mid]);
        root->left = sortedArrayToBST(nums, start, mid - 1);
        root->right = sortedArrayToBST(nums, mid + 1, end);
        return root;
    }

    // Méthode pour rechercher un élément donné dans un ABR
    bool search(TreeNode* root, int val) {
        if (!root) return false;
        if (root->val == val) return true;
        if (val < root->val) return search(root->left, val);
        else return search(root->right, val);
    }

    // Méthodes pour effectuer les parcours pré-ordre, in-ordre et post-ordre d'un ABR
    void preorder(TreeNode* root) {
        if (!root) return;
        cout << root->val << " ";
        preorder(root->left);
        preorder(root->right);
    }

    void inorder(TreeNode* root) {
        if (!root) return;
        inorder(root->left);
        cout << root->val << " ";
        inorder(root->right);
    }

    void postorder(TreeNode* root) {
        if (!root) return;
        postorder(root->left);
        postorder(root->right);
        cout << root->val << " ";
    }

    // Méthode pour insérer un nouvel élément dans un ABR
    TreeNode* insert(TreeNode* root, int val) {
        if (!root) return new TreeNode(val);
        if (val < root->val) root->left = insert(root->left, val);
        else root->right = insert(root->right, val);
        return root;
    }

    // Méthode pour supprimer un élément donné d'un ABR
    TreeNode* deleteNode(TreeNode* root, int key) {
        if (!root) return nullptr;
        if (key < root->val) root->left = deleteNode(root->left, key);
        else if (key > root->val) root->right = deleteNode(root->right, key);
        else {
            if (!root->left) {
                TreeNode* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                TreeNode* temp = root->left;
                delete root;
                return temp;
            }
            TreeNode* temp = root->right;
            while (temp->left) temp = temp->left;
            root->val = temp->val;
            root->right = deleteNode(root->right, temp->val);
        }
        return root;
    }

    // Méthode pour calculer la hauteur d'un ABR
    int height(TreeNode* root) {
        if (!root) return 0;
        return max(height(root->left), height(root->right)) + 1;
    }

    // Méthode pour vérifier si un arbre donné est un ABR valide
    bool isValidBST(TreeNode* root, TreeNode* minNode = nullptr, TreeNode* maxNode = nullptr) {
        if (!root) return true;
        if ((minNode && root->val <= minNode->val) || (maxNode && root->val >= maxNode->val))
            return false;
        return isValidBST(root->left, minNode, root) && isValidBST(root->right, root, maxNode);
    }
};

int main() {
    // Exemple d'utilisation de la classe BinarySearchTree
    BinarySearchTree bst;
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7};
    TreeNode* root = bst.sortedArrayToBST(nums, 0, nums.size() - 1);
    cout << "Height of BST: " << bst.height(root) << endl;
    cout << "Inorder traversal: ";
    bst.inorder(root);
    cout << endl;
    cout << "Is valid BST? " << (bst.isValidBST(root) ? "Yes" : "No") << endl;
    root = bst.insert(root, 8);
    cout << "Inorder traversal after insertion: ";
    bst.inorder(root);
    cout << endl;
    root = bst.deleteNode(root, 5);
    cout << "Inorder traversal after deletion: ";
    bst.inorder(root);
    cout << endl;
    return 0;
}

