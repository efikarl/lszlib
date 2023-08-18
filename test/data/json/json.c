#include <lszlib.h>

void testcase_jboo(void)
{
    int              r      = LSZ_RET_0_ERR;
    json_t           json   = NULL;
    char            *target = NULL;
    char            *expect = NULL;
    long             data   = 0;


    json = json_new();
    TESTCASE_ASSERT_BOOL (
        json != NULL,
    "testcase_jboo@json_new");

    target = json_decode(json);
    expect = "{}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jboo@json_new.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jboo_get(json, "bool", &data);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jboo@jboo_get.bool.r[+]");

    r = jboo_add(json, "bool", 0);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jboo@jboo_add.data=0.r");

    target = json_decode(json);
    expect = "{\"bool\":false}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jboo@jboo_add.data=0.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jboo_get(json, "bool", &data);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jboo@jboo_get.data=0.r");
    TESTCASE_ASSERT_BOOL (
        data == 0,
    "testcase_jboo@jboo_get.data=0.v");

    r = jboo_set(json, "bool", 1);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jboo@jboo_set.data=1.r");
    target = json_decode(json);
    expect = "{\"bool\":true}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jboo@jboo_set.data=1.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jboo_get(json, "bool", &data);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jboo@jboo_get.data=1.r");
    TESTCASE_ASSERT_BOOL (
        data == 1,
    "testcase_jboo@jboo_get.data=1.v");

    r = json_del(json, "bool");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jboo@json_del.bool.r");

    r = jboo_get(json, "bool", &data);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jboo@jboo_get.bool.r[-]");

    target = json_decode(json);
    expect = "{}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jboo@json_del.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    json_free(json);
}

void testcase_jnum(void)
{
    int              r      = LSZ_RET_0_ERR;
    json_t           json   = NULL;
    char            *target = NULL;
    char            *expect = NULL;
    long             data   = 0;


    json = json_new();
    TESTCASE_ASSERT_BOOL (
        json != NULL,
    "testcase_jnum@json_new");

    target = json_decode(json);
    expect = "{}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jnum@json_new.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jnum_get(json, "number", &data);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jnum@jnum_get.number.r[+]");

    r = jnum_add(json, "number", 0);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jnum@jnum_add.data=0.r");

    target = json_decode(json);
    expect = "{\"number\":0}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jnum@jnum_add.data=0.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jnum_get(json, "number", &data);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jnum@jnum_get.data=0.r");
    TESTCASE_ASSERT_BOOL (
        data == 0,
    "testcase_jnum@jnum_get.data=0.v");

    r = jnum_set(json, "number", 1);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jnum@jnum_set.data=1.r");
    target = json_decode(json);
    expect = "{\"number\":1}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jnum@jnum_set.data=1.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jnum_get(json, "number", &data);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jnum@jnum_get.data=1.r");
    TESTCASE_ASSERT_BOOL (
        data == 1,
    "testcase_jnum@jnum_get.data=1.v");

    r = json_del(json, "number");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jnum@json_del.number.r");

    r = jnum_get(json, "number", &data);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jnum@jnum_get.number.r[-]");

    target = json_decode(json);
    expect = "{}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jnum@json_del.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    json_free(json);
}

void testcase_jnul(void)
{
    int              r      = LSZ_RET_0_ERR;
    json_t           json   = NULL;
    char            *target = NULL;
    char            *expect = NULL;

    json = json_new();
    TESTCASE_ASSERT_BOOL (
        json != NULL,
    "testcase_jnul@json_new");

    target = json_decode(json);
    expect = "{}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jnul@json_new.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jnul_get(json, "null");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jnul@jnul_get.null.r[+]");

    r = jnul_add(json, "null");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jnul@jnul_add.r");

    target = json_decode(json);
    expect = "{\"null\":null}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jnul@jnul_add.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jnul_get(json, "null");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jnul@jnul_get.r");

    //
    // null can be set to other type
    //
    r = jstr_set(json, "null", "world");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jnul@jnul_set.data=world.r");
    target = json_decode(json);
    expect = "{\"null\":\"world\"}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jnul@jstr_set.data=world.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jstr_get(json, "null", &target);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jnul@jstr_get.data=world.r");
    TESTCASE_ASSERT_DATA (
        target, "world", 0,
    "testcase_jnul@jstr_get.data=world.v");
    if (target) {
        free (target);
        target = NULL;
    }

    r = json_del(json, "null");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jnul@json_del.null.r");

    r = jstr_get(json, "null", &target);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jnul@jstr_get.null.r[-]");

    target = json_decode(json);
    expect = "{}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jnul@json_del.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    json_free(json);
}

void testcase_jstr(void)
{
    int              r      = LSZ_RET_0_ERR;
    json_t           json   = NULL;
    char            *target = NULL;
    char            *expect = NULL;

    json = json_new();
    TESTCASE_ASSERT_BOOL (
        json != NULL,
    "testcase_jstr@json_new");

    target = json_decode(json);
    expect = "{}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jstr@json_new.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jstr_get(json, "string", &target);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jstr@jstr_get.string.r[+]");

    r = jstr_add(json, "string", "hello");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jstr@jstr_add.data=hello.r");

    target = json_decode(json);
    expect = "{\"string\":\"hello\"}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jstr@jstr_add.data=hello.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jstr_get(json, "string", &target);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jstr@jstr_get.data=hello.r");
    TESTCASE_ASSERT_DATA (
        target, "hello", 0,
    "testcase_jstr@jstr_get.data=hello.v");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jstr_set(json, "string", "world");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jstr@jstr_set.data=world.r");
    target = json_decode(json);
    expect = "{\"string\":\"world\"}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jstr@jstr_set.data=world.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jstr_get(json, "string", &target);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jstr@jstr_get.data=world.r");
    TESTCASE_ASSERT_DATA (
        target, "world", 0,
    "testcase_jstr@jstr_get.data=world.v");
    if (target) {
        free (target);
        target = NULL;
    }

    r = json_del(json, "string");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jstr@json_del.string.r");

    r = jstr_get(json, "string", &target);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jstr@jstr_get.string.r[-]");

    target = json_decode(json);
    expect = "{}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jstr@json_del.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    json_free(json);
}

void testcase_jobj(void)
{
    int              r      = LSZ_RET_0_ERR;
    json_t           json0  = NULL;
    json_t           json1  = NULL;
    json_t           json2  = NULL;
    char            *target = NULL;
    char            *expect = NULL;
    long             data   = 0;

    json0 = json_new();
    TESTCASE_ASSERT_BOOL (
        json0 != NULL,
    "testcase_jstr@json_new");

    target = json_decode(json0);
    expect = "{}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jstr@json_new.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    json1 = json_new();
    TESTCASE_ASSERT_BOOL (
        json1 != NULL,
    "testcase_jobj@json_new");
    target = json_decode(json1);
    expect = "{}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jstr@json_new.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = jobj_get(json0, "object", &json1);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jobj@jobj_get.object.r[+]");

    r = jobj_add(json0, "object", json1);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jobj@jobj_add.object.r");

    target = json_decode(json0);
    expect = "{\"object\":{}}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jobj@jobj_add.object.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }
    
    r = jobj_get(json0, "object", &json1);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jobj@jobj_get.object.r");

    r = json_del(json0, "object");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jobj@json_del.object.r");

    r = jobj_get(json0, "object", &json1);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jobj@jobj_get.object.r[-]");

    json1 = json_new();
    json2 = json_new();
    jboo_add(json1, "bool", 1);
    jboo_add(json2, "bool", 0);
    jobj_add(json0, "obj1", json1);
    jobj_add(json1, "obj2", json2);
    target = json_decode(json0);
    expect = "{\"obj1\":{\"bool\":true,\"obj2\":{\"bool\":false}}}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jobj@jobj_add.object.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    jboo_get(json0, "obj1/bool", &data);
    TESTCASE_ASSERT_BOOL (
        data == 1,
    "testcase_jobj@jboo_get.object.data");

    jboo_get(json0, "obj1/obj2/bool", &data);
    TESTCASE_ASSERT_BOOL (
        data == 0,
    "testcase_jobj@jboo_get.object.data");

    jboo_set(json0, "obj1/bool", 0);
    jboo_set(json0, "obj1/obj2/bool", 1);
    target = json_decode(json0);
    expect = "{\"obj1\":{\"bool\":false,\"obj2\":{\"bool\":true}}}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_jobj@jboo_set.object.json_decode");
    if (target) {
        free (target);
        target = NULL;
    }

    r = json_del(json0, "obj1");
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_0_ERR,
    "testcase_jobj@json_del.obj1.r");

    r = jobj_get(json0, "obj1", &json1);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jobj@jobj_get.obj1.r");

    r = jobj_get(json0, "obj1/obj2", &json2);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jobj@jobj_get.obj1/obj2.r");

    r = jboo_get(json0, "obj1/obj2/bool", &data);
    TESTCASE_ASSERT_BOOL (
        r == LSZ_RET_E_NFD,
    "testcase_jobj@jboo_get.obj1/obj2/bool.r");

    json_free(json0);
}

void testcase_apis(void)
{
    int              r      = 0;
    json_t           json0  = json_new();
    json_t           json1  = NULL;
    char            *target = NULL;
    char            *expect = NULL;

    jstr_add(json0, "name", "karl");
    jstr_add(json0, "pswd", "p0ss");
    target = json_decode(json0);
    expect = "{\"name\":\"karl\",\"pswd\":\"p0ss\"}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_apis@json_decode.add.string");
    if (target) {
        free (target);
        target = NULL;
    }

    jobj_add(json0, "sub-object", NULL);
    jobj_get(json0, "sub-object", &json1);
    jnum_add(json1, "lo", 1);
    jnum_add(json1, "hi", 3);
    target = json_decode(json0);
    expect = "{\"name\":\"karl\",\"pswd\":\"p0ss\",\"sub-object\":{\"lo\":1,\"hi\":3}}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_apis@json_decode.add.sub_object");
    if (target) {
        free (target);
        target = NULL;
    }

    jboo_add(json0, "sub-object/table[0]", 1);
    jboo_add(json0, "sub-object/table[]" , 0);
    jboo_add(json0, "sub-object/table[2]", 1);
    target = json_decode(json0);
    expect = "{\"name\":\"karl\",\"pswd\":\"p0ss\",\"sub-object\":{\"lo\":1,\"hi\":3,\"table\":[true,false,true]}}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_apis@json_decode.add.array.bool");
    if (target) {
        free (target);
        target = NULL;
    }

    jnum_add(json0, "sub-object/array[]", 1);
    jnum_add(json0, "sub-object/array[]", 2);
    jnum_add(json0, "sub-object/array[]", 3);
    target = json_decode(json0);
    expect = "{\"name\":\"karl\",\"pswd\":\"p0ss\",\"sub-object\":{\"lo\":1,\"hi\":3,\"table\":[true,false,true],\"array\":[1,2,3]}}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_apis@json_decode.add.array.nums");
    if (target) {
        free (target);
        target = NULL;
    }

    jobj_add(json0, "objects[]", NULL);
    jstr_add(json0, "objects[0]/string", "a");
    jobj_add(json0, "objects[]", NULL);
    jstr_add(json0, "objects[1]/string", "b");
    target = json_decode(json0);
    expect = "{\"name\":\"karl\",\"pswd\":\"p0ss\",\"sub-object\":{\"lo\":1,\"hi\":3,\"table\":[true,false,true],\"array\":[1,2,3]},\"objects\":[{\"string\":\"a\"},{\"string\":\"b\"}]}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_apis@json_decode.add.array.objects");
    if (target) {
        free (target);
        target = NULL;
    }

    jnul_add(json0, "reserve");

    json_del(json0, "name");
    json_del(json0, "pswd");
    target = json_decode(json0);
    expect = "{\"sub-object\":{\"lo\":1,\"hi\":3,\"table\":[true,false,true],\"array\":[1,2,3]},\"objects\":[{\"string\":\"a\"},{\"string\":\"b\"}],\"reserve\":null}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_apis@json_decode.del.string");
    if (target) {
        free (target);
        target = NULL;
    }

    json_del(json0, "sub-object/table[1]");
    target = json_decode(json0);
    expect = "{\"sub-object\":{\"lo\":1,\"hi\":3,\"table\":[true,true],\"array\":[1,2,3]},\"objects\":[{\"string\":\"a\"},{\"string\":\"b\"}],\"reserve\":null}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_apis@json_decode.del.array.member");
    if (target) {
        free (target);
        target = NULL;
    }

    json_del(json0, "sub-object");
    target = json_decode(json0);
    expect = "{\"objects\":[{\"string\":\"a\"},{\"string\":\"b\"}],\"reserve\":null}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_apis@json_decode.del.object");
    if (target) {
        free (target);
        target = NULL;
    }

    json_del(json0, "objects[0]");
    json_del(json0, "objects[0]");
    target = json_decode(json0);
    expect = "{\"objects\":[],\"reserve\":null}";
    TESTCASE_ASSERT_DATA (
        target, expect, 0,
    "testcase_apis@json_decode.del.array.index.shall.decrement");
    if (target) {
        free (target);
        target = NULL;
    }
    json_del(json0, "objects");
    json_del(json0, "reserve");

    json_free(json0);
}

char * read_file_to_buffer(char *path)
{
    FILE       *file        = NULL;
    long        file_size   = 0;
    size_t      read_size   = 0;
    char       *buffer      = NULL;

    file = fopen(path, "rb");
    if (file == NULL) {
        perror("error: fopen");
        goto eofn;
    }
    fseek(file, 0, SEEK_END);
    file_size =  ftell(file);
    rewind(file);

    buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        perror("error: malloc");
        goto eofn;
    }

    read_size = fread(buffer, 1, file_size, file);
    if (read_size != file_size) {
        perror("error: fread");
        goto eofn;
    }
    buffer[file_size] = '\0';

    return buffer;

eofn:
    if (buffer != NULL) {
        free (buffer);
        buffer = NULL;
    }
    return buffer;
}

void testcase_json_file_encode_decode(void)
{
    FILE       *file        = NULL;
    long        file_size   = 0;
    size_t      read_size   = 0;
    char       *target      = NULL;
    char       *expect      = NULL;
    json_t      json        = NULL;

    target = read_file_to_buffer(".target.json");
    if (target == NULL) {
        goto eofn;
    }

    json = json_encode(target);
    free(target);
    target = json_decode(json);

    expect = read_file_to_buffer(".expect.json");
    if (target == NULL) {
        goto eofn;
    }

    TESTCASE_ASSERT_DATA (
        expect, target, 0,
    "testcase_json_file_encode_decode:\n%s\n", target);

    {
        int     r   = LSZ_RET_0_ERR;
        long    num = 0;
        char   *str = NULL;

        jstr_get(json, "object.array[0]/name", &str);
        TESTCASE_ASSERT_DATA (
            str, "1st", 0,
        "testcase_json_file_encode_decode@jstr_get.object.array[0]/name");
        if (str) {
            free (str);
            str = NULL;
        }

        jstr_get(json, "object.array[1]/name", &str);
        TESTCASE_ASSERT_DATA (
            str, "2nd", 0,
        "testcase_json_file_encode_decode@jstr_get.object.array[1]/name");
        if (str) {
            free (str);
            str = NULL;
        }
        jstr_set(json, "object.array[1]/name","9th");
        jstr_get(json, "object.array[1]/name", &str);
        TESTCASE_ASSERT_DATA (
            str, "9th", 0,
        "testcase_json_file_encode_decode@jstr_set.object.array[1]/name");
        if (str) {
            free (str);
            str = NULL;
        }

        jnum_get(json, "object.array[0]/nums[0][1][1]", &num);
        TESTCASE_ASSERT_BOOL (
            num == 15,
        "testcase_json_file_encode_decode@jnum_get.object.array[0]/nums[0][1][1]");
        jnum_set(json, "object.array[0]/nums[0][1][1]", 1024);
        jnum_get(json, "object.array[0]/nums[0][1][1]", &num);
        TESTCASE_ASSERT_BOOL (
            num == 1024,
        "testcase_json_file_encode_decode@jnum_set.object.array[0]/nums[0][1][1]");
    }

eofn:
    if (target != NULL) {
        free(target);
    }
    if (expect != NULL) {
        free(expect);
    }
    if (file != NULL) {
        fclose(file);
    }
    if (json != NULL) {
        json_free(json);
    }
}

int main(int argc, char const *argv[])
{
    lsz_testcase_do {
        testcase_jboo();
        testcase_jnum();
        testcase_jnul();
        testcase_jstr();
        testcase_jobj();
        testcase_apis();
        testcase_json_file_encode_decode();
    } lsz_testcase_done;
}
