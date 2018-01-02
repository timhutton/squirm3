// SquirmCellProperties.h

class SquirmCellProperties
{
public:

    SquirmCellProperties(char t,int s);

    char GetType() { return this->type; } 
    int GetState() { return this->state; }

    void SetState(int s) { 
        this->state=s; 
    }

    void SetType(char t)
    {
        this->type = t;
    }

    static char GetRandomType();

    static void EIGHT(int i,int &x,int &y);
    static void FOUR(int i,int &x,int &y);

protected:

    char type;
    int state;
};
