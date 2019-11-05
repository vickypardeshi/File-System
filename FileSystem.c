#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#include<string.h>
#include<dirent.h>
#include <unistd.h>
#define MAX 256
struct stat st;
struct dirent *de;

struct file
{
  char name[20];
  struct file *left,*right;
};

struct file *head=NULL;

void create_parent_directory()
{
  char dirname[20];
  struct file *temp;
  temp=(struct file *)malloc(sizeof(struct file));
  printf("Enter directory name:-");
  scanf("%s",temp->name);
  temp->left=temp->right=NULL;
  if(stat(temp->name,&st)==-1)   //check dir present or not if not return -1
    {
      mkdir(temp->name,0777);
      printf("\nDirectory is created\n");
    }
  else
      printf("\nDirectory already exist\n");
  head=temp;
}

struct file*  create_directory(struct file *head)
{
  char dirname[20];
  struct file *temp;
  temp=(struct file *)malloc(sizeof(struct file));
  printf("Enter dir name:-");
  scanf("%s",temp->name);
  temp->left=temp->right=NULL;
  if(stat(temp->name,&st)==-1)   //check dir present or not if not return -1
    {
      mkdir(temp->name,0777);
      printf("\nDirectory is created\n");
      
      if(head==NULL)
	head=temp;

      else
	{
	  struct file *curr=head,*parent=head;
	  while(curr)
	    {
	      parent=curr;
	      if(strcmp(temp->name,curr->name)<0)
		curr=curr->left;
	      else if(strcmp(temp->name,curr->name)>0)
		curr=curr->right;
	      else break;
	    }
	  if(strcmp(temp->name,parent->name)<0)
	    parent->left=temp;
	  else if(strcmp(temp->name,parent->name)>0)
	    parent->right=temp;
	}
    }
  else
    printf("\nDirectory already exist\n");
  return head;
}

struct file * create_file(struct file *head,char f_name[])
{
  FILE *fp;
  char buff[10000];
  char dname[20];
  struct file *temp;
  temp=(struct file*)malloc(sizeof(struct file));
  strcpy(temp->name,f_name);
  temp->left=temp->right=NULL;
  fp=fopen(temp->name,"w");
  if(fp==NULL)
    printf("\nFile not created\n");
  else
    {
      printf("\nFile is created\n");
      if(head==NULL)
	head=temp;
	
      else
	{
	  struct file *curr=head,*parent;
	  while(curr)
	    {
	      parent=curr;
	      if(strcmp(temp->name,curr->name)<0)
		curr=curr->left;
	      else if(strcmp(temp->name,curr->name)>0)
		curr=curr->right;
	      else break;
	    }
	  if(strcmp(temp->name,parent->name)<0)
	    parent->left=temp;
	  else if(strcmp(temp->name,parent->name)>0)
	    parent->right=temp;
	}
	
    }
  return head;
}

void write_file()
{
  char file_name[20],buffer[100];
  printf("\nEnter file name:-");
  scanf("%s",file_name);
  FILE *fp;
  fp=fopen(file_name,"a");
  if(fp==NULL)
    printf("\nNo such file exist\n");
  else
    {
      printf("Enter text :-");
      scanf("%s",buffer);
      fputs(buffer,fp);
      printf("\nData are inserted...\n");
    }
}

void read_file()
{
  char fname[20],line[100];
  FILE *fp;
  printf("\nEnter a file name:-");
  scanf("%s",fname);
  fp=fopen(fname,"r");
  if(fp==NULL)
    printf("\nDon't exist?\n");
  else
    {
      printf("\nFile content are..\n");
      while(fgets(line,100,fp)!=NULL)
	printf("%s",line);
      printf("\n");
    }
}


void display_directory()
{
  char dname[20];
  printf("Enter dirname:-");
  scanf("%s",dname);
  DIR *dr=opendir(dname); 
  
  if(dr==NULL)  // opendir returns NULL if couldn't open directory 
    {
      printf("\nNo such a directory\n");
      return ;
    }
  printf("\nFiles & Directories are:-\n");
  while((de=readdir(dr))!=NULL) 
    printf("%s\n",de->d_name); 
  closedir(dr);
}

void inorder(struct file *root)                
{
  if(root!=NULL)
    {
      inorder(root->left);
      printf("%s\t",root->name);
      inorder(root->right);
    }
}

int search_directory(char name[])
{
  if(strcmp(name,head->name)==0)
    return 1;
  struct file *temp=head->left;
  while(temp)
    {
      if(strcmp(name,temp->name)==0)
	  return 1;
      else if(strcmp(name,temp->name)<0)
	temp=temp->left;
      else
	temp=temp->right;
    }
  return 0;
}

int search_file(char name[])
{
  if(strcmp(name,head->name)==0)
    return 1;
  struct file *temp=head->right;
  while(temp)
    {
      if(strcmp(name,temp->name)==0)
	  return 1;
      else if(strcmp(name,temp->name)<0)
	temp=temp->left;
      else
	temp=temp->right;
    }
  return 0;
}

void delete_directory(char rm_dir_name[])
{
  int check;
  check=rmdir(rm_dir_name);
  if(!check)
    printf("Directory Deleted successfully\n");
  else
    printf("No such directory\n");
}

void delete_file(char rm_file[])
{
  if(remove(rm_file) == 0) 
      printf("File Deleted successfully\n"); 
   else
      printf("No such file\n");
}

void delete_directories(char name[])
{
  int found;
  found=search_directory(name);
  if(found==1)
    {
      struct file *curr=head->left,*parent=head->left;
      while(strcmp(curr->name,name)!=0)
	{
	  parent=curr;
	  if(strcmp(name,curr->name)<0)
	    curr=curr->left;
	  else
	    curr=curr->right;
	}
      
      delete_directory(name);
	
      if(curr->left==NULL && curr->right==NULL)
	{
	  if(curr==parent->left)
	      parent->left=NULL;
	  else
	    parent->right=NULL;
	  free(curr);
	}
      
      else if(curr->left!=NULL && curr->right==NULL)
	{
	  if(curr==parent)
	    {
	      struct file *s;
	      s=curr->left;
	      strcpy(parent->name,s->name);
	      parent->left=s->left;
	      free(s);
	      return;
	    }
	  else if(curr==parent->left)
	      parent->left=curr->left;
	  else
	      parent->right=curr->left;
	  curr->left==NULL;
	  free(curr);
	}
      
      else if(curr->left==NULL && curr->right!=NULL)
	{
	  if(curr==parent)
	    {
	      struct file *s;
	      s=curr->right;
	      strcpy(parent->name,s->name);
	      parent->right=s->right;
	      free(s);
	      return;
	    }
	  if(curr==parent->left)
	      parent->left=curr->right;
	  else
	      parent->right=curr->right;
	  curr->right==NULL;
	  free(curr);
	}
      
      else if(curr->left!=NULL && curr->right!=NULL)
	{
	  struct file *t1=curr->right,*t2;
	  while(t1->left!=NULL)
	    {
	      t2=t1;
	      t1=t1->left;
	    }
	  strcpy(curr->name,t1->name);
	  if(t1->left==NULL && t1->right!=NULL)
	      t2->left=t1->right;
	  else
	    t2->left=NULL;
	  free(t1);
	}
    }
  
  else
    printf("\nNo such directory exist\n\n");
}

void delete_files(char name[])
{
  int found;
  found=search_file(name);
  if(found==1)
    {
      struct file *curr=head->right,*parent=head->right;
      while(strcmp(curr->name,name)!=0)
	{
	  parent=curr;
	  if(strcmp(name,curr->name)<0)
	    curr=curr->left;
	  else
	    curr=curr->right;
	}
      
      delete_file(name);
	
      if(curr->left==NULL && curr->right==NULL)
	{
	  if(curr==parent->left)
	      parent->left=NULL;
	  else
	    parent->right=NULL;
	  free(curr);
	}
      
      else if(curr->left!=NULL && curr->right==NULL)
	{
	  if(curr==parent)
	    {
	      struct file *s;
	      s=curr->left;
	      strcpy(parent->name,s->name);
	      parent->left=s->left;
	      free(s);
	      return;
	    }
	  else if(curr==parent->left)
	      parent->left=curr->left;
	  else
	      parent->right=curr->left;
	  curr->left==NULL;
	  free(curr);
	}
      
      else if(curr->left==NULL && curr->right!=NULL)
	{
	  if(curr==parent)
	    {
	      struct file *s;
	      s=curr->right;
	      strcpy(parent->name,s->name);
	      parent->right=s->right;
	      free(s);
	      return;
	    }
	  if(curr==parent->left)
	      parent->left=curr->right;
	  else
	      parent->right=curr->right;
	  curr->right==NULL;
	  free(curr);
	}
      
      else if(curr->left!=NULL && curr->right!=NULL)
	{
	  struct file *t1=curr->right,*t2;
	  while(t1->left!=NULL)
	    {
	      t2=t1;
	      t1=t1->left;
	    }
	  strcpy(curr->name,t1->name);
	  if(t1->left==NULL && t1->right!=NULL)
	      t2->left=t1->right;
	  else
	    t2->left=NULL;
	  free(t1);
	}
    }
  
  else
    printf("\nNo such file exist\n\n");
}


void copy()
{
  int ch;
  FILE *fp1, *fp2;
  char source[20], target[20];
  printf("\nEnter your source file name:-");
  scanf("%s",source);
  printf("Enter your destination file name:-");
  scanf("%s",target);
  fp1=fopen(source,"r");
  fp2=fopen(target,"w");
  if(!fp1)
    {
      printf("\nNo source file exist\n");
      fclose(fp2);
    }
  if(!fp2)
    printf("\nUnable to open target file to write\n");

  if(fp2 && fp1)
    {
      // head=create_file(head,target);
      while((ch=fgetc(fp1))!=EOF)
	fputc(ch, fp2);
      printf("\nFile copied...!\n");
    }
  fclose(fp1);
  fclose(fp2);
}

void  move()
{
  int ch;
  FILE *fp1, *fp2;
  char source[MAX],target[MAX];
  printf("\nEnter your source file name:-");
  scanf("%s",source);
  printf("Enter your destination file name:-");
  scanf("%s",target);
  fp1=fopen(source,"r");
  fp2=fopen(target,"w");
  if(!fp1)
    {
      printf("\nNo source file exist\n");
      fclose(fp2);
    }

  if(!fp2)
      printf("Unable to open target file to write\n");
      
  if(fp1 && fp2)
    {
      // head=create_file(head,target);
      while((ch=fgetc(fp1))!=EOF)
	fputc(ch, fp2);
      printf("\nFile moved...!\n");
      //delete_file(source);   //delete source file
    }
  
  fclose(fp1);
  fclose(fp2);
  remove(source);
}

void path_directory()
{
  char filename[20];
  printf("\nEnter dir name:-");
  scanf("%s",filename);
  char* path = realpath(filename,NULL);
  if(path==NULL)
    printf("\nNo such directory\n");
  else
    printf("\nPath:- %s\n",path);  	 
}

void path_file()
{
  char filename[20];
  printf("\nEnter file name:-");
  scanf("%s",filename);
  char* path = realpath(filename,NULL);
  if(path==NULL)
    printf("\nNo such file\n");
  else
    printf("\npath:- %s\n", path);   	
}

void rename_directory()
{
  char old_name[20],new_name[20];
  int value;
  printf("\nEnter old directory name:-");
  scanf("%s",old_name);
  printf("Enter new directory name:-");
  scanf("%s",new_name); 
  value=rename(old_name,new_name); 
  if(!value) 
    printf("\nDirectory name changed successfully...!\n");
  else
    perror("Error\n");  
}

void rename_file()
{
  char old_name[20],new_name[20];
  int value;
  printf("\nEnter old file name:-");
  scanf("%s",old_name);
  printf("Enter new file name:-");
  scanf("%s",new_name); 
  value=rename(old_name,new_name); 
  if(!value) 
    printf("\nFile name changed successfully...!\n");
  else
    perror("Error\n");  
}


long int findSize(const char *file_name)
{
  struct stat st;  
  if(stat(file_name,&st)==0)
    return (st.st_size);
  else
    return -1;
}

void size_file()
{
  char fname[10];
  FILE *fp; 
  long int size=0;  
  printf("\nEnter file name:-");
  scanf("%s",fname);
  size=findSize(fname); 
  if(size!=-1)
    printf("\nFile size is:- %ld byte\n",size);
  else
    perror("Error\n");
}

void main()
{
  int ch,found;
  char dir_name[20],file_name[20];
  create_parent_directory();
  while(1)
    {
      printf("\n1.Create Directory");
      printf("\n2.Create File");
      printf("\n3.Open File for Writting");
      printf("\n4.Open File for Reading");
      printf("\n5.Display parent Directory");
      printf("\n6.Display child Directory");    //particular directory
      printf("\n7.Search Directory");
      printf("\n8.Search File");
      printf("\n9.Delete Directory");
      printf("\n10.Delete File");
      printf("\n11.Copy data from one file to another file");  //head->right=copy(head->right);
      printf("\n12.Move data from one file to another file");   //head->right=move(head->right);
      printf("\n13.Find directory Path");
      printf("\n14.Find file path");
      printf("\n15.Rename of a Directory");
      printf("\n16.Rename of a file");
      printf("\n17.Find size of a file");
      printf("\n18.Exit");
      printf("\nEnter your choice:-");
      scanf("%d",&ch);
      switch(ch)
	{
	case 1:
	  head->left=create_directory(head->left);
	  break;
	  
	case 2:
	  printf("\nEnter file name:-");
	  scanf("%s",file_name);
	  head->right=create_file(head->right,file_name);
	  break;
	  
	case 3:
	  write_file();
	  break;

	case 4:
	  read_file();
	  break;
	  
	case 5:
	  printf("\nDirectories & Files are:-\n");
	  inorder(head);
	  printf("\n");
	  break;

	case 6:
	  display_directory();
	  break;

	case 7:
	  printf("\nEnter directory name:-");
	  scanf("%s",dir_name);
	  found=search_directory(dir_name);
	  if(found==1)
	    printf("\nDirectory present in file system\n");
	  else
	    printf("\nNo such directory exist\n");
	  break;

	case 8:
	  printf("\nEnter file name:-");
	  scanf("%s",file_name);
	  found=search_file(file_name);
	  if(found==1)
	    printf("\nFile present in file system\n");
	  else
	    printf("\nNo such file exist\n");
	  break;
	  break;

	case 9:
	  printf("\nEnter directory name:-");
	  scanf("%s",dir_name);
	  delete_directories(dir_name);
	  break;

	case 10:
	  printf("\nEnter file name:-");
	  scanf("%s",file_name);
	  delete_files(file_name);
	  break;

	case 11:
	  copy();
	  break;

	case 12:
	  move();
	  break;

	case 13:
	  path_directory();
	  break;

	case 14:
	  path_file();
	  break;

	case 15:
	  rename_directory();
	  break;

	case 16:
	  rename_file();
	  break;

	case 17:
	  size_file();
	  break;

	case 18:
	  exit(0);
	  break;

	default:printf("\nInvalid choice \n");
	  break;
	}
    }
}
