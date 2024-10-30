#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <jansson.h>

unsigned long long convert_base_to_decimal(const char *value, int base) {
    return strtoull(value, NULL, base);
}

int parse_json(const char *filename, int *n, int *k, int *x, unsigned long long *y) {
    json_t *root, *keys, *val_obj;
    json_error_t error;
    
    root = json_load_file(filename, 0, &error);
    if (!root) {
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
        return 1;
    }
    
    keys = json_object_get(root, "keys");
    *n = json_integer_value(json_object_get(keys, "n"));
    *k = json_integer_value(json_object_get(keys, "k"));

    int i = 0;
    char index_str[10];
    json_t *base, *value;
    json_object_foreach(root, index_str, val_obj) {
        if (strcmp(index_str, "keys") == 0) continue;
        
        x[i] = atoi(index_str);
        base = json_object_get(val_obj, "base");
        value = json_object_get(val_obj, "value");
        
        int base_val = atoi(json_string_value(base));
        const char *value_str = json_string_value(value);
        
        y[i] = convert_base_to_decimal(value_str, base_val);
        i++;
    }
    json_decref(root);
    return 0;
}

unsigned long long lagrange_interpolation(int *x, unsigned long long *y, int k) {
    unsigned long long result = 0;
    
    for (int i = 0; i < k; i++) {
        double term = y[i];
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= -x[j];
                term /= (double)(x[i] - x[j]);
            }
        }
        result += (unsigned long long)term;
    }
    return result;
}

int main() {
    int n, k;
    int x[10];
    unsigned long long y[10];
    
    if (parse_json("testcase1.json", &n, &k, x, y) != 0) {
        return 1;
    }
    unsigned long long constant_term = lagrange_interpolation(x, y, k);
    printf("Constant term (c) for Test Case 1: %llu\n", constant_term);
    
    if (parse_json("testcase2.json", &n, &k, x, y) != 0) {
        return 1;
    }
    constant_term = lagrange_interpolation(x, y, k);
    printf("Constant term (c) for Test Case 2: %llu\n", constant_term);
    
    return 0;
}
