#include<vector>
#include<ctime>
using std::vector;

namespace Skiplist{

template <typename V>

class skipListNode{
public:
    int key;
    V value;
    vector<skipListNode*> nexts;
    skipListNode(int l , V v = 0,int k =0){
        this->key = k;
        this->value = v;
        nexts.resize(l,nullptr);         //为什么要level
    }
};
template <typename V>
class skipList{
private:
    int maxLevel;
    int curLevel;
    skipListNode* head;
    
public:
    skipList(int ml = 16){
        this->maxLevel = ml;
        this->curLevel = 0;
        this->head = new skipListNode(maxLevel);                //这个头节点不用传值 ，相当于这个头指针，以来是有 默认16个尾巴
        srand(time(0));
    }
    ~skipList();
    int randomLevel(){
        int i = 1 ;
        while(rand() % 2){
            i++;
        }
        return maxLevel > i ? maxLevel : i;
    }

    void insert(int key,V v){                                //这里用void 感觉还是考虑重复值吧
        vector<skipListNode*> update(maxLevel,head);        //相当于默认情况下 16 个位置，每个位置都是16 个尾巴
                                                            //相当于重复刚刚的过程，但是只是在每一层循环 更新以下这个update数组
        auto cur = head;                                    //从头节点的最高层开始
        int tmpLevel = maxLevel -1;
        while(tmpLevel >=0){
            //每一层走不动了就走下一层
            while(cur->nexts[tmpLevel] && cur->nexts[tmpLevel] < key){
                cur = cur->nexts[tmpLevel];                 //退出去就是走不动了
            }
            //就是在这更新以下
            update[tmpLevel] = cur;
            --tmpLevel;
        }                                                    
        int levelToInsert = randomLevel();
        this->curLevel = max(curLevel,levelToInsert);       //更新以下总体的最大高度
        //增加就完事了   update在 新节点前面
        auto newNode = new skipListNode(maxLevel,v,key);
        for( int i = 0 ; i < levelToInsert ; ++i){
            newNode->nexts[i] = update[i]->nexts[i];
            update[i]->nexts[i] = newNode;
        }
        return ;
    }
    bool erase(int key , V v){
        vector<skipListNode*> update(maxLevel,head);        //相当于默认情况下 16 个位置，每个位置都是16 个尾巴
                                                            //相当于重复刚刚的过程，但是只是在每一层循环 更新以下这个update数组
        auto cur = head;                                    //从头节点的最高层开始
        int tmpLevel = maxLevel -1;
        while(tmpLevel >=0){
            //每一层走不动了就走下一层
            while(cur->nexts[tmpLevel] && cur->nexts[tmpLevel] < key){
                cur = cur->nexts[tmpLevel];                 //退出去就是走不动了
            }
            //就是在这更新以下
            update[tmpLevel] = cur;
            --tmpLevel;
        }
        cur = cur->netxs[0];
        //上面的过程就是search 加一个update
        //首先看看这玩意在不在，不再删个皮，简单偷个懒，这里因为不止有值域，还有value域，其实还应该去最下面这一层遍历找到value
        //so be it
        if(!cur || cur->nexts[0] != key){
            return ; //bye  不需要做什么
        }
        for ( int i = 0 ; i < curLevel ;++i){
            //从下面往上删除
            //直到update连的不再是你cur
            if(update[i]->nexts[i] != cur){
                break;
            }
            update[i]->nexts[i] = cur->nexts[i];
        }
        delete cur;
        while(curLevel > 1 && !update[curLevel -1]){
            --curLevel;
            //更新以下最新的高度，因为删除那个节点之后
            //update每一层在最右边的映射，如果空，降低高度
        }
        return true;
    }
    V search(int key ,V v ){                                //先就不管后的值域了
        auto cur = head;                                    //从头节点的最高层开始
        int tmpLevel = maxLevel -1;
        while(tmpLevel >=0){
            //每一层走不动了就走下一层
            while(cur->nexts[tmpLevel] && cur->nexts[tmpLevel] < key){
                cur = cur->nexts[tmpLevel];                 //退出去就是走不动了
            }
            --tmpLevel;
        }
        cur = cur->nexts[0];
        if(cur->netxs[0] && key == cur->nexts[0]){          //如果还有尾巴，就看尾巴后面是不是要找的
            return true;                                    //其他情况直接false
        }
        return false;
    }
};
}
