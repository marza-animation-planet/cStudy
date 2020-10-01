#include <stdlib.h>
#include <stdio.h>

typedef struct _Table2
{
   float *values;
   int columns;
   int rows;
} Table2;

const int T2_OK = 0;
const int T2_ERR_OUT_OF_RANGE = 1;
const int T2_ERR_NULL_POINTER = 2;
const int T2_ERR_INVALID_SIZE = 3;
const int T2_ERR_LAST = 4;

const char* t2_errstr(int code)
{
   static const char* sErrs[] = {"No error",
                                 "Index out of range",
                                 "Invalid pointers",
                                 "Invalid size"};

   if (code < 0 || code >= T2_ERR_LAST)
   {
      return "Unknown error";
   }
   else
   {
      return sErrs[code];
   }
}

Table2* t2_new(int nr, int nc, float initval)
{
   if (nr < 0 || nc < 0)
   {
      return NULL;
   }

   Table2 *t = (Table2*) malloc(sizeof(Table2));

   t->rows = nr;
   t->columns = nc;

   int nvals = nr * nc;

   if (nvals == 0)
   {
      t->values = NULL;
   }
   else
   {
      t->values = (float*) malloc(nvals * sizeof(float));
      for (int i=0; i<nvals; ++i)
      {
         t->values[i] = initval;
      }
   }

   return t;
}

int t2_clear(Table2 *t)
{
   if (!t)
   {
      return T2_ERR_NULL_POINTER;
   }

   if (t->values)
   {
      free(t->values);
      t->values = NULL;
   }

   t->rows = 0;
   t->columns = 0;

   return T2_OK;
}

int t2_free(Table2 **t)
{
   if (!t)
   {
      return T2_ERR_NULL_POINTER;
   }
   else
   {
      int retcode = t2_clear(*t);

      if (retcode != T2_OK)
      {
         return retcode;
      }

      free(*t);
      *t = NULL;

      return T2_OK;
   }
}


int t2_get(Table2 *t, int r, int c, float *ret)
{
   int idx;

   if (!ret || !t)
   {
      return T2_ERR_NULL_POINTER;
   }
   if (r < 0 || r >= t->rows || c < 0 || c >= t->columns)
   {
      return T2_ERR_OUT_OF_RANGE;
   }

   idx = (r * t->columns) + c;
   *ret = t->values[idx];

   return T2_OK;
}

int t2_set(Table2 *t, int r, int c, float val)
{
   int idx;

   if (!t)
   {
      return T2_ERR_NULL_POINTER;
   }
   if (r < 0 || r >= t->rows || c < 0 || c >= t->columns)
   {
      return T2_ERR_OUT_OF_RANGE;
   }

   idx = (r * t->columns) + c;
   t->values[idx] = val;

   return T2_OK;
}

int t2_resize(Table2 *t, int nr, int nc, float initval)
{
   if (!t)
   {
      return T2_ERR_NULL_POINTER;
   }

   if (nr < 0 || nc < 0)
   {
      return T2_ERR_INVALID_SIZE;
   }

   int nvals = nr * nc;

   if (nvals == 0)
   {
      t2_clear(t);
      return T2_OK;
   }

   float *nv = (float*) malloc(nvals * sizeof(float));
   int rcopy = (nr < t->rows ? nr : t->rows);
   int ccopy = (nc < t->columns ? nc : t->columns);
   int retcode;
   int idx;
   float val;

   idx = 0;

   for (int r=0; r<rcopy; ++r)
   {
      for (int c=0; c<ccopy; ++c)
      {
         if ((retcode = t2_get(t, r, c, &val)) != T2_OK)
         {
            free(nv);
            return retcode;
         }
         nv[idx++] = val;
      }

      for (int c=ccopy; c<nc; ++c)
      {
         nv[idx++] = initval;
      }
   }

   for (int r=rcopy; r<nr; ++r)
   {
      for (int c=0; c<nc; ++c)
      {
         nv[idx++] = initval;
      }
   }

   free(t->values);
   t->values = nv;
   t->rows = nr;
   t->columns = nc;

   return T2_OK;
}


int t2_print(Table2 *t)
{
   if (!t)
   {
      return T2_ERR_NULL_POINTER;
   }

   float val;
   int retcode;

   fprintf(stdout, "Table %dx%d\n", t->rows, t->columns);

   for (int r=0; r<t->rows; ++r)
   {
      fprintf(stdout, "  [");
      for (int c=0; c<t->columns; ++c)
      {
         if ((retcode = t2_get(t, r, c, &val)) != T2_OK)
         {
            fprintf(stderr, "%s\n", t2_errstr(retcode));
            return retcode;
         }
         fprintf(stdout, (c+1 < t->columns ? "%f, " : "%f"), val);
      }
      fprintf(stdout, "]\n");
   }

   return T2_OK;
}

// ---

int main(int argc, char **argv)
{
   int retcode;
   float val;

   // create table
   Table2 *t = t2_new(4, 2, 0.0f);

   // initialize table
   val = 1.0f;
   for (int r=0; r<t->rows; ++r)
   {
      for (int c=0; c<t->columns; ++c)
      {
         t2_set(t, r, c, val);
         val += 1.0f;
      }
   }

   t2_print(t);

   fprintf(stdout, "---\n");

   // print table by columns
   for (int c=0; c<t->columns; ++c)
   {
      fprintf(stdout, "column[%d] = {", c);
      for (int r=t->rows-1; r>=0; --r)
      {
         if ((retcode = t2_get(t, r, c, &val)) != T2_OK)
         {
            fprintf(stderr, "%s\n", t2_errstr(retcode));
            t2_free(&t);
            exit(1);
         }
         fprintf(stdout, (r > 0 ? "row[%d]=%f, " : "row[%d]=%f"), r, val);
      }
      fprintf(stdout, "}\n");
   }

   fprintf(stdout, "---\n");

   t2_resize(t, 12, 4, 0.33f);
   t2_print(t);

   t2_free(&t);

   return 0;
}
