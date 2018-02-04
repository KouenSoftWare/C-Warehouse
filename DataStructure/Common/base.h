#ifndef JK_DS_BASE_H  
#define JK_DS_BASE_H

#include <string>
#include <memory>
#include <iostream>

namespace jk{
namespace ds{


class NodeError{};
class NodeKeyTypeError : NodeError{};
class NodeFindError : NodeError{};

template<class VALUE>
struct Node{
    public:
        enum NodeStatus{Delete, Active, Null};
        enum KeyType{String, Int, Unkown};

    private:
        std::string sKey; 
        int iKey;
        std::shared_ptr<VALUE> value;
        KeyType keyType;
        NodeStatus status;

    public:
        Node()
            :value(), keyType(KeyType::Unkown), status(NodeStatus::Active){ value = std::make_shared<VALUE>();}
        Node(std::string k)
            :sKey(k), value(), keyType(KeyType::String), status(NodeStatus::Active){ value = std::make_shared<VALUE>();}
        Node(int k)
            :iKey(k), value(), keyType(KeyType::Int), status(NodeStatus::Active){ value = std::make_shared<VALUE>();}
        Node(std::string k, const VALUE& v)
            :sKey(k), value(), keyType(KeyType::String), status(NodeStatus::Active){ value = std::make_shared<VALUE>(v);}
        Node(int k, const VALUE& v)
            :iKey(k), value(), keyType(KeyType::Int), status(NodeStatus::Active){ value = std::make_shared<VALUE>(v);}
        Node(const Node& n)
            :sKey(n.sKey), iKey(n.iKey), value(n.value), keyType(n.keyType), status(n.status){}

        Node& operator=(const Node& n){
            if (this == &n){
                return *this;
            }

            this->iKey = n.iKey; 
            this->sKey = n.sKey; 
            *(this->value.get()) = *n.value.get(); 
            this->status = n.status; 
            this->keyType = n.keyType; 
            return *this;
        }

        void del(){
            this->status = NodeStatus::Delete;
        }

        int GetHash(int modNumber=0) const{
            int h;
            switch (this->keyType){
                case Node<VALUE>::KeyType::String:
                    for (auto i: this->sKey){
                        h += (int(i)*10);
                    }
                    if (modNumber != 0)
                        h %= modNumber;
                    break;
                case Node<VALUE>::KeyType::Int:
                    if (modNumber != 0)
                        h = this->iKey % modNumber;
                    else
                        h = this->iKey;
                    break;
                default:
                    throw NodeKeyTypeError();
            }
            if (h < 0)
                return h*-1;
            else
                return h;
        }

        bool operator==(const Node<VALUE>& n){
            if (n.keyType != keyType)
                return false;
            switch (keyType){
                case Node<VALUE>::KeyType::String:
                    return sKey == n.sKey;
                    break;
                case Node<VALUE>::KeyType::Int:
                    return iKey == n.iKey;
                    break;
                default:
                    throw NodeKeyTypeError();
            }
        }
        
        bool isAlive(){
            return status == NodeStatus::Active; 
        }

        VALUE* val(){
            return const_cast<VALUE*>(((const Node<VALUE> *)this)->val());
        }

        const VALUE* const val() const{
            return value.get(); 
        }
};

template<class VALUE>
class DSBase{
    public:
        virtual ~DSBase() { };

        virtual Node<VALUE>* pop(){
            return nullptr; 
        } 

        virtual void insert(std::string key, const VALUE& value){
            Node<VALUE> n(key, value);
            this->insert(n);
        }
        virtual void insert(int key, const VALUE& value){
            Node<VALUE> n(key, value);
            this->insert(n);
        }

        virtual void remove(std::string key){
            Node<VALUE> n(key);
            this->remove(n);
        }
        virtual void remove(int key){
            Node<VALUE> n(key);
            this->remove(n);
        }

        virtual VALUE* find(std::string key) {
            Node<VALUE> n(key);
            return this->find(n);
        }
        virtual VALUE* find(int key) {
            Node<VALUE> n(key);
            return this->find(n);
        }

        virtual const VALUE * const find(std::string key) const {
            Node<VALUE> n(key);
            return this->find(n);
        }
        virtual const VALUE * const find(int key) const {
            Node<VALUE> n(key);
            return this->find(n);
        }

        virtual VALUE* find(const Node<VALUE>& n){
            return const_cast<VALUE*>(((const DSBase<VALUE> *)this)->find(n));
        }

        virtual const VALUE& operator[](std::string key) const{
            return *(this->find(key));
        }
        virtual const VALUE& operator[](int key) const{
            return *(this->find(key));
        }
        virtual VALUE& operator[](std::string key) {
            return *(const_cast<VALUE*>(((const DSBase<VALUE> *)this)->find(key)));
        }
        virtual VALUE& operator[](int key) {
            return *(const_cast<VALUE*>(((const DSBase<VALUE> *)this)->find(key)));
        }

        virtual void insert(const Node<VALUE>&) = 0;
        virtual void remove(const Node<VALUE>&) = 0;
        virtual const VALUE* const find(const Node<VALUE>&) const = 0;
        virtual void clear() = 0;
};

template <class T>
T max(T a, T b){
    return a > b ? a : b;
}


} // ds namespace end
} // jk namespace end

#endif
