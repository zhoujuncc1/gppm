#include <Python.h>
#include <numpy/arrayobject.h>

static const int N_SPICIES = 13;
static const int N_PARAMS = 42;
double FunctionForApaf_1Turnover(double prod_0, double param_0, double param_1) 	//Function for Apaf-1 turnover
{return  param_1-param_0*prod_0;}
double FunctionForCaspase9Turnover(double prod_0, double param_0, double param_1) 	//Function for Caspase 9 turnover
{return  param_1-param_0*prod_0;}
double FunctionForXiapTurnover(double prod_0, double param_0, double param_1) 	//Function for Xiap turnover
{return  param_1-param_0*prod_0;}
double FunctionForCaspase3Turnover(double prod_0, double param_0, double param_1) 	//Function for Caspase 3 turnover
{return  param_1-param_0*prod_0;}

static PyObject * odefun(PyObject *self, PyObject *args)
{
    double t;
    PyObject *p_obj;
    PyObject *x_obj;
    if (!PyArg_ParseTuple(args, "dOO", &t, &p_obj, &x_obj))
        return NULL;

    PyArrayObject *p_array = PyArray_FROM_OTF(p_obj, NPY_DOUBLE, NPY_IN_ARRAY);
    PyArrayObject *x_array = PyArray_FROM_OTF(x_obj, NPY_DOUBLE, NPY_IN_ARRAY);

    if (p_array == NULL || x_array == NULL)
        return NULL;

    double *p = (double*)PyArray_DATA(p_array);
    double *x = (double*)PyArray_DATA(x_array);
    double *dx = (double*)malloc(N_SPICIES * sizeof(double));
    if (dx == NULL)
        return NULL;
    
dx[0] = -(p[11] * x[5] * x[0] - p[12] * x[11]) *p[0]-(p[13] * x[4] * x[0] - p[14] * x[7]) *p[0]-(p[23] * x[1] * x[0] - p[24] * x[12]) *p[0]-(p[15] * x[3] * x[0] - p[16] * x[8]) *p[0]-(p[17] * x[6] * x[0] - p[18] * x[10]) *p[0]+FunctionForXiapTurnover(x[0], p[29], p[30])*p[0];	//
dx[1] = (p[3] * x[9] * x[5]) *p[0]+(p[4] * x[9] * x[4]) *p[0]-(p[23] * x[1] * x[0] - p[24] * x[12]) *p[0]+(p[7] * x[9] * x[3]) *p[0]+(p[8] * x[9] * x[6]) *p[0]-(p[36] * x[1]) *p[0];	//
dx[2] = -(p[1] * x[2] * x[5] - p[2] * x[4]) *p[0]-(p[19] * x[11] * x[2] - p[20] * x[7]) *p[0]-(p[9] * x[3] * x[2] - p[10] * x[6]) *p[0]-(p[21] * x[8] * x[2] - p[22] * x[10]) *p[0]+FunctionForApaf_1Turnover(x[2], p[25], p[26])*p[0];	//
dx[3] = (p[5] * x[5] * x[1]) *p[0]-(p[9] * x[3] * x[2] - p[10] * x[6]) *p[0]-(p[15] * x[3] * x[0] - p[16] * x[8]) *p[0]-(p[39] * x[3]) *p[0];	//
dx[4] = (p[1] * x[2] * x[5] - p[2] * x[4]) *p[0]-(p[13] * x[4] * x[0] - p[14] * x[7]) *p[0]-(p[6] * x[4] * x[1]) *p[0]-(p[33] * x[4]) *p[0];	//
dx[5] = -(p[1] * x[2] * x[5] - p[2] * x[4]) *p[0]-(p[11] * x[5] * x[0] - p[12] * x[11]) *p[0]-(p[5] * x[5] * x[1]) *p[0]+FunctionForCaspase9Turnover(x[5], p[27], p[28])*p[0];	//
dx[6] = (p[6] * x[4] * x[1]) *p[0]+(p[9] * x[3] * x[2] - p[10] * x[6]) *p[0]-(p[17] * x[6] * x[0] - p[18] * x[10]) *p[0]-(p[40] * x[6]) *p[0];	//
dx[7] = (p[13] * x[4] * x[0] - p[14] * x[7]) *p[0]+(p[19] * x[11] * x[2] - p[20] * x[7]) *p[0]-(p[32] * x[7]) *p[0];	//
dx[8] = (p[15] * x[3] * x[0] - p[16] * x[8]) *p[0]-(p[21] * x[8] * x[2] - p[22] * x[10]) *p[0]-(p[38] * x[8]) *p[0];	//
dx[9] = -(p[3] * x[9] * x[5]) *p[0]-(p[4] * x[9] * x[4]) *p[0]-(p[7] * x[9] * x[3]) *p[0]-(p[8] * x[9] * x[6]) *p[0]+FunctionForCaspase3Turnover(x[9], p[34], p[35])*p[0];	//
dx[10] = (p[17] * x[6] * x[0] - p[18] * x[10]) *p[0]+(p[21] * x[8] * x[2] - p[22] * x[10]) *p[0]-(p[41] * x[10]) *p[0];	//
dx[11] = (p[11] * x[5] * x[0] - p[12] * x[11]) *p[0]-(p[19] * x[11] * x[2] - p[20] * x[7]) *p[0]-(p[31] * x[11]) *p[0];	//
dx[12] = (p[23] * x[1] * x[0] - p[24] * x[12]) *p[0]-(p[37] * x[12]) *p[0];	//
    int nd = 1;
    npy_intp dims[] = {N_SPICIES};
    PyArrayObject *dx_array = PyArray_SimpleNewFromData(nd, dims, NPY_DOUBLE, dx);
    if (dx_array == NULL)
        return NULL;

    return dx_array;
}

static PyMethodDef methods[] = {
    {"odefun", odefun, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}
};

void initapoptosis()
{
    PyObject *m = Py_InitModule3("apoptosis", methods, "");
    if (m == NULL) return;

    import_array();
}
