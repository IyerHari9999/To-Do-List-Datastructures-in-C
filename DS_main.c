#include<stdio.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include<assert.h>

struct entry
{
    struct date
    {
        int day;
        int month;
        int year;
    }dd;
    char sub[20];
    int expno;
    struct entry *next;
}*start=NULL,*tail=NULL;

/returns address of newly created block of memory/
struct entry* create()
{
    struct entry *data;
    data=(struct entry*)malloc(sizeof(struct entry));
    printf("\nnenter data of submission dd/mm/yyyy: ");
    scanf("%d%[/]%d%[/]%d",&data->dd.day,&data->dd.month,&data->dd.year);
    printf("\nenter name of subject: ");
    scanf("%s",data->sub);
    printf("\nenter experiment number: ");
    scanf("%d",&data->expno);

    data->next=NULL;
    return data;

}
/*
this function counts no of nodes to allocate that many bytes*number of bytes required for each instance of structure for dat file
*/
int NoOfNodes(struct entry *front)
{
    struct entry *temp=front;
    int count=0;
    while(temp!=NULL)
    {
        count++;
        temp=temp->next;
    }
    return count;
}
/*
This function is used to add new experiments to to do list in  a sorted manner as per the submission dates.
An experiment with a nearer due date is stored first
*/

void add()
{
    struct entry *temp=create();
    if(start==NULL)
    {
        start=temp;
        tail=start;
    }
    else
    {
        struct entry *tempnode; //as a temporary variable to hold address of start

        struct entry *current,*prev=start;
        current=start;              //to traverse the list

        int nodesize=temp->dd.year*1000+temp->dd.month*100+temp->dd.day;    //numerical representation of due date of the node to be inserted
        while(prev!=NULL&&current!=NULL)
        {
            int currentsize=current->dd.year*1000+current->dd.month*100+current->dd.day;  //numerical representation of the due date of the node iterated
            if(currentsize>=nodesize)       // i.e. if the new node has due date prior to the current iterated node
            {
                if(current==start)          //if the new node is to be inserted at the start itself
                {
                    tempnode=start;
                    start=temp;
                    start->next=tempnode;
                }
                else if(prev->next=NULL)//prev=tail i.e if the new node has to be inserted at the very end of the list
                {
                    prev->next=temp;
                    tail=temp;

                }
                else                    //here, we insert the new node between the current and the previous nodes
                {
                    prev->next=temp;
                    prev=prev->next;
                    prev->next=current;
                }
                return;
            }

            else            //if the newly created node has due date somewhere after the current node
            {
              prev=current;
              current=current->next;
            }

        }
        prev->next=temp;
        tail=prev;
        return;
    }
    int c=NoOfNodes(start);//to count number of nodes for the fwrite function so as to allocate that many bytes for the stream dynamically




    FILE *fp;           //file pointer
    temp=start;
    if((fp=fopen("abc.dat","a"))!=NULL)
    {
        fwrite(temp,sizeof(struct entry),c,fp);
        /*
        fwrite is used to write to file
        fwrite takes 4 arguements:the pointer to the stream,size of each variable,total no of such sizes,file ptr

        */
        fclose(fp);
    }
    else
    {
     printf("\nerror in opening the File");
     exit(0);
    }
}


void display()
{

    struct entry *temp=start;
    printf("\n-----------------------------------------------------------\n");
    printf("\nDate of Submission\tSubject\tExperiment Number\n-----------------------------------------------------------\n");

    while(temp!=NULL)
    {

        printf("%d/%d/%d\t\t%s\t%d\n",temp->dd.day,temp->dd.month,temp->dd.year,temp->sub,temp->expno);

        temp=temp->next;
    }
}
/*
this function deletes the experiment numbers that have crossed due dates of submission.
it takes the current cpu time as input to compare the current time and date of submission
any experiment with due date before the current system time is removed automatically
*/
struct entry* del()
{

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("\ncurrent date and time: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1,tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    int currentTime=(tm.tm_year + 1900)*1000+(tm.tm_mon + 1)*100+tm.tm_mday; //current date of system
    while(start!=NULL)
    {
        int subtime=start->dd.year*1000+start->dd.month*100+start->dd.day;  //sub date of start
        printf("\nOverdue Experiment:\n");
        if(subtime<currentTime) //crossed deadline
        {
            /Print the current node/
            printf("%d/%d/%d\t\t%s\t%d\n",start->dd.day,start->dd.month,start->dd.year,start->sub,start->expno);
            struct entry *temp=start;
            //push(temp);
            free(start);
            start=start->next;
        }
        else return start;
    }
}
/* this function lets the user to remove an experiment manually. it asks the subjetc name and the exp no. as input
to search and delete the experimnet if found*/
void removemanual()
{
        char rmsub[20];
        int rmexp;
        char ch='n';
        printf("\nenter name of subject: ");
        scanf("%s",rmsub);
        printf("\nenter experiment number: ");
        scanf("%d",&rmexp);
        struct entry* curr=start,*prev;
        while(curr!=NULL)
        {
            if(!strcmp(curr->sub,rmsub))
            {
                if(curr->expno==rmexp)
                {
                    printf("\nexperiment found!! Sure to delete?(y/n)");
                    ch=getche();
                    if(ch=='n')
                    {
                        printf("\ncanceled\n");
                        break;
                    }
                    if(curr==start)
                    {
                        start=start->next;
                        free(curr);
                        curr=NULL;
                        printf("\n\tfreed\n");
                        break;
                    }
                    prev->next=curr->next;
                    printf("\n\tfreed\n");
                    free(curr);



                }
            }
            prev=curr;
            curr=curr->next;
        }
printf("\nexperiment not found!\n");
}


main()
{
    int ch=1;
    while(ch!=0)
    {

        printf("\nPress 1 to add");
        printf("\nPress 2 to display");
        printf("\nPress 3 to remove experiments automatically");
        printf("\npress 4 to remove experiments manually");
        printf("\npress 0 to exit\n");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:add();
                    break;
            case 2: display();
                    break;
            case 3: del();
                    break;
            case 4: removemanual();
                    break;
            case 0:
                    exit(0);
            default: printf("\ninvalid choice");

        }
    }

}