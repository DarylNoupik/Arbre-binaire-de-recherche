#include <iostream>
#include <vector>

using namespace std;

const int T = 3; // Ordre du B-Tree

// Définition de la classe TreeNode représentant un nœud dans un B-Tree
class TreeNode {
public:
    bool leaf;
    vector<int> keys;
    vector<TreeNode*> children;

    TreeNode(bool leafNode) : leaf(leafNode) {}

    // Fonction pour insérer une clé dans le nœud
    void insert(int key) {
        int i = keys.size() - 1;
        if (leaf) {
            keys.push_back(0);
            while (i >= 0 && key < keys[i]) {
                keys[i + 1] = keys[i];
                i--;
            }
            keys[i + 1] = key;
        } else {
            while (i >= 0 && key < keys[i]) i--;
            int idx = i + 1;
            if (children[idx]->keys.size() == 2 * T - 1) {
                splitChild(idx, children[idx]);
                if (key > keys[idx]) idx++;
            }
            children[idx]->insert(key);
        }
    }

    // Fonction pour séparer un enfant plein en deux enfants
    void splitChild(int idx, TreeNode* y) {
        TreeNode* z = new TreeNode(y->leaf);
        z->keys.resize(T - 1);

        for (int j = 0; j < T - 1; j++)
            z->keys[j] = y->keys[j + T];

        if (!y->leaf) {
            z->children.resize(T);
            for (int j = 0; j < T; j++)
                z->children[j] = y->children[j + T];
        }

        y->keys.resize(T - 1);

        children.insert(children.begin() + idx + 1, z);
        keys.insert(keys.begin() + idx, y->keys[T - 1]);
    }

    // Fonction pour supprimer une clé du nœud
    void remove(int key) {
        int idx = 0;
        while (idx < keys.size() && keys[idx] < key) idx++;

        if (idx < keys.size() && keys[idx] == key) { // La clé est présente dans ce nœud
            if (leaf) {
                keys.erase(keys.begin() + idx);
            } else {
                if (children[idx]->keys.size() >= T) {
                    // Cas 1: La clé a au moins T clés dans le sous-arbre gauche
                    int pred = getPredecessor(idx);
                    keys[idx] = pred;
                    children[idx]->remove(pred);
                } else if (children[idx + 1]->keys.size() >= T) {
                    // Cas 2: La clé a au moins T clés dans le sous-arbre droit
                    int succ = getSuccessor(idx);
                    keys[idx] = succ;
                    children[idx + 1]->remove(succ);
                } else {
                    // Cas 3: Combinaison de sous-arbres
                    merge(idx);
                    children[idx]->remove(key);
                }
            }
        } else {
            if (leaf) {
                cout << "La clé " << key << " n'est pas présente dans l'arbre." << endl;
                return;
            }
            bool flag = (idx == keys.size());
            if (children[idx]->keys.size() < T)
                fill(idx);
            if (flag && idx > keys.size())
                children[idx - 1]->remove(key);
            else
                children[idx]->remove(key);
        }
    }

    // Fonction pour récupérer le prédécesseur d'une clé dans le sous-arbre gauche
    int getPredecessor(int idx) {
        TreeNode* cur = children[idx];
        while (!cur->leaf) cur = cur->children[cur->keys.size()];
        return cur->keys.back();
    }

    // Fonction pour récupérer le successeur d'une clé dans le sous-arbre droit
    int getSuccessor(int idx) {
        TreeNode* cur = children[idx + 1];
        while (!cur->leaf) cur = cur->children[0];
        return cur->keys[0];
    }

    // Fonction pour combiner deux enfants
    void merge(int idx) {
        TreeNode* child = children[idx];
        TreeNode* sibling = children[idx + 1];

        child->keys.push_back(keys[idx]);

        for (int i = 0; i < sibling->keys.size(); ++i)
            child->keys.push_back(sibling->keys[i]);

        if (!child->leaf) {
            for (int i = 0; i < sibling->children.size(); ++i)
                child->children.push_back(sibling->children[i]);
        }

        keys.erase(keys.begin() + idx);
        children.erase(children.begin() + idx + 1);
        delete sibling;
    }

    // Fonction pour remplir un enfant qui a moins de T clés
    void fill(int idx) {
        if (idx != 0 && children[idx - 1]->keys.size() >= T)
            borrowFromPrev(idx);
        else if (idx != keys.size() && children[idx + 1]->keys.size() >= T)
            borrowFromNext(idx);
        else {
            if (idx != keys.size())
                merge(idx);
            else
                merge(idx - 1);
        }
    }

    // Fonction pour emprunter une clé du précédent voisin
    void borrowFromPrev(int idx) {
        TreeNode* child = children[idx];
        TreeNode* sibling = children[idx - 1];

        for (int i = child->keys.size() - 1; i >= 0; --i)
            child->keys[i + 1] = child->keys[i];

        if (!child->leaf) {
            for (int i = child->children.size() - 1; i >= 0; --i)
                child->children[i + 1] = child->children[i];
        }

        child->keys[0] = keys[idx - 1];

        if (!child->leaf)
            child->children[0] = sibling->children[sibling->keys.size()];

        keys[idx - 1] = sibling->keys.back();

        child->keys.push_back(keys[idx - 1]);

        if (!child->leaf)
            sibling->children.pop_back();
        sibling->keys.pop_back();
    }

    // Fonction pour emprunter une clé du voisin suivant
    void borrowFromNext(int idx) {
        TreeNode* child = children[idx];
        TreeNode* sibling = children[idx + 1];

        child->keys.push_back(keys[idx]);

        if (!child->leaf)
            child->children.push_back(sibling->children[0]);

        keys[idx] = sibling->keys[0];

        sibling->keys.erase(sibling->keys.begin());
        if (!sibling->leaf)
            sibling->children.erase(sibling->children.begin());
    }

    // Fonction pour afficher le contenu du nœud
    void print() {
        for (int i = 0; i < keys.size(); ++i)
            cout << keys[i] << " ";
        cout << endl;
    }
};

// Définition de la classe BTree représentant un B-Tree
class BTree {
private:
    TreeNode* root;

public:
    BTree() : root(nullptr) {}

    // Fonction pour insérer une clé dans le B-Tree
    void insert(int key) {
        if (!root) {
            root = new TreeNode(true);
            root->keys.push_back(key);
        } else {
            if (root->keys.size() == 2 * T - 1) {
                TreeNode* temp = new TreeNode(false);
                temp->children.push_back(root);
                temp->splitChild(0, root);
                int i = 0;
                if (temp->keys[0] < key)
                    i++;
                temp->children[i]->insert(key);
                root = temp;
            } else
                root->insert(key);
        }
    }

    // Fonction pour supprimer une clé du B-Tree
    void remove(int key) {
        if (!root) {
            cout << "L'arbre est vide." << endl;
            return;
        }
        root->remove(key);
        if (root->keys.empty()) {
            TreeNode* temp = root;
            if (root->leaf)
                root = nullptr;
            else
                root = root->children[0];
            delete temp;
        }
    }

    // Fonction pour afficher le contenu du B-Tree
    void print() {
        if (root != nullptr) root->print();
    }
};

int main() {
    BTree btree;

    // Insertion dans le B-Tree
    btree.insert(10);
    btree.insert(20);
    btree.insert(5);
    btree.insert(6);
    btree.insert(12);
    btree.insert(30);
    btree.insert(7);
    btree.insert(17);

    cout << "Contenu du B-Tree après insertion: ";
    btree.print();
    cout << endl;

    // Suppression dans le B-Tree
    btree.remove(12);
    cout << "Contenu du B-Tree après suppression de la clé 12: ";
    btree.print();
    cout << endl;

    return 0;
}
