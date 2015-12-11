//Matt Layman CS341 h4.c
//Implements FIFO, LRU, and OPT page replacement

#include <stdio.h>
//#define NELEMS(x) (sizeof(x) / sizeof(x[0]));

int n,pg[30],fr[10], psize;
void fifo();
void optimal();
void lru();
int main()
{
  int i, ch;
  printf("\nEnter total number of pages:");
  scanf("%d",&n);
  printf("\nEnter page reference stream:\n");
  for(i=0;i<n;i++)      //accepting sequence
    scanf("%d",&pg[i]);
  printf("\nEnter number of page frames:");
  scanf("%d",&psize);
  //n = NELEMS(pg);
  do
  {
    printf("\nEnter page replacement algorithm(1.FIFO, 2.LRU, 3.OPT, 4.EXIT): ");
    scanf("%d",&ch);
    switch(ch)
    {
      case 1: fifo();
        break;

      case 2: optimal();
        break;

      case 3: lru();
        break;

    }
  }while(ch!=4);
  getchar();
  return 0;
}

void fifo()        //fifo page replacement
{
  int i,f,r,s,count,flag,num;
  f=0;
  r=0;
  s=0;
  flag=0;
  count=0;
  //printf("\nEnter number of page frames:");
  //scanf("%d",&psize);
  printf("\nFIFO Page Replacement\n");
  for(i=0;i<psize;i++)  
  {
    fr[i]=-1;
  }
  while(s<n)  
  {
    flag=0;
    num=pg[s];
    for(i=0;i<psize;i++)
    {
      if(num==fr[i])
      {
        s++;
        flag=1;
        break;
      }
    }
    if(flag==0)
    {
      if(r<psize)
      { 
        fr[r]=pg[s];
        r++;
        s++;
        count++;
      }
      else
      {
        if(f<psize)
        {
          fr[f]=pg[s];
          s++;
          f++;
          count++;
        }
        else 
          f=0;
      }
    }
    printf("\n");
    for(i=0;i<psize;i++)
    {
      printf("%d\t",fr[i]);
    }
  }
printf("\nNumber of page faults = %d",count);
printf("\nNumber of page references = %d",n);
printf("\nFault ratio = %.2f",count/(double)n);
getchar();
}

void optimal()  //optimal page replacement
{
  int count[10],i,j,k,fault,f,flag,temp,current,c,max,m,cnt,p,x;
  fault=0;
  k=0;
  //printf("\nEnter frame size:");
  //scanf("%d",&f);
  //initilizing distance and frame array
  f = psize;
  printf("\nOptimal Page Replacement\n");
  for(i=0;i<f;i++)
  {
    count[i]=0;
    fr[i]=-1;
  }
  for(i=0;i<n;i++)
  {
    flag=0;
    temp=pg[i];
    for(j=0;j<f;j++)      
    {
      if(temp==fr[j])
      {
        flag=1;
        break;
      }
    }
    if((flag==0)&&(k<f))
    {
      fault++;
      fr[k]=temp;
      k++;
    }
    else if((flag==0)&&(k==f))
    {
      fault++;      
      for(cnt=0;cnt<f;cnt++)      
      {   
        current=fr[cnt];
        for(c=i;c<n;c++)
        {
          if(current!=pg[c])
            count[cnt]++;
          else
            break;
        }
      }
      max=0;
      for(m=0;m<f;m++)
      {
        if(count[m]>max)
        {
          max=count[m];
          p=m;
        }
      }
      fr[p]=temp;
    }
    printf("\n");     
    for(x=0;x<f;x++)
    {
      printf("%d\t",fr[x]);
    }
  }
  printf("\nNumber of page faults = %d",fault);
  printf("\nNumber of page references = %d", n);
  printf("\nFault ratio = %.2f", fault/(double)n);
  getchar();
}

void lru()        //lru page replacement
{
  int count[10],i,j,k,fault,f,flag,temp,current,c,max,m,cnt,p,x;
  fault=0;
  k=0;
  //printf("\nEnter frame size:");
  //scanf("%d",&f);
  //initilizing distance and frame array
  f = psize;
  printf("\nLRU Page Replacement\n");
  for(i=0;i<f;i++)
  {
    count[i]=0;
    fr[i]=-1;
  }
  for(i=0;i<n;i++)
  {
    flag=0;
    temp=pg[i];
    for(j=0;j<f;j++)      
    {
      if(temp==fr[j])
      {
        flag=1;
        break;
      }
    }
    if((flag==0)&&(k<f))
    {
      fault++;
      fr[k]=temp;
      k++;
    }
    else if((flag==0)&&(k==f))
    {
      fault++;      
      for(cnt=0;cnt<f;cnt++)      
      {   
        current=fr[cnt];
        for(c=i;c>0;c--)
        {
          if(current!=pg[c])
            count[cnt]++;
          else
            break;
        }
      }
      max=0;
      for(m=0;m<f;m++)
      {
        if(count[m]>max)
        {
          max=count[m];
          p=m;
        }
      }
      fr[p]=temp;
    }
    printf("\n");     
    for(x=0;x<f;x++)
    {
      printf("%d\t",fr[x]);
    }
  }
  printf("\nNumber of page faults = %d",fault);
  printf("\nNumber of page references = %d", n);
  printf("\nFault ratio = %.2f", fault/(double)n);
  getchar();
}
