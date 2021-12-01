#pragma once

class IBaseFileSystem;
class CUtlBuffer;
class Color;
class key_values;
class key_values_dump_context;
typedef void* file_handle_t;
typedef void* get_symbol_proc_t;

// single byte identifies a xbox kv file in binary format
// strings are pooled from a searchpath/zip mounted symbol table
#define KV_BINARY_POOLED_FORMAT 0xAA

#define FOR_EACH_SUBKEY(kvRoot, kvSubKey) for (KeyValues* kvSubKey = kvRoot->GetFirstSubKey(); kvSubKey != nullptr; kvSubKey = kvSubKey->GetNextKey())

#define FOR_EACH_TRUE_SUBKEY(kvRoot, kvSubKey) for (KeyValues* kvSubKey = kvRoot->GetFirstTrueSubKey(); kvSubKey != nullptr; kvSubKey = kvSubKey->GetNextTrueSubKey())

#define FOR_EACH_VALUE(kvRoot, kvValue) for (KeyValues* kvValue = kvRoot->GetFirstValue(); kvValue != nullptr; kvValue = kvValue->GetNextValue())

class key_values
{
public:
    static void set_use_growable_string_table(bool bUseGrowableTable);

    key_values(const char* set_name)
    {
    }

    class auto_delete
    {
    public:
        explicit inline auto_delete(key_values* p_key_values) : kv(p_key_values)
        {
        }
        explicit inline auto_delete(const char* pch_kv_name) : kv(new key_values(pch_kv_name))
        {
        }
        inline ~auto_delete(void)
        {
            if (kv)
                kv->delete_this();
        }
        inline void asssign(key_values* pKeyValues)
        {
            kv = pKeyValues;
        }
        key_values* operator->()
        {
            return kv;
        }
        operator key_values*()
        {
            return kv;
        }

    private:
        auto_delete(auto_delete const& x);            // forbid
        auto_delete& operator=(auto_delete const& x); // forbid
        key_values* kv;
    };

    // Quick setup constructors
    key_values(const char* set_name, const char* first_key, const char* first_value);
    key_values(const char* set_name, const char* first_key, const wchar_t* first_value);
    key_values(const char* set_name, const char* first_key, int first_value);
    key_values(const char* set_name, const char* first_key, const char* first_value, const char* second_key, const char* second_value);
    key_values(const char* set_name, const char* first_key, int first_value, const char* second_key, int second_value);

    // Section name
    const char* get_name() const;
    void set_name(const char* set_name);

    // gets the name as a unique int
    int get_name_symbol() const
    {
        return key_name;
    }

    // File access. Set UsesEscapeSequences true, if resource file/buffer uses Escape Sequences (eg \n, \t)
    void uses_escape_sequences(bool state); // default false
    void uses_conditionals(bool state);     // default true
    bool load_from_file(void* filesystem, const char* resource_name, const char* path_id = nullptr);
    bool save_to_file(void* filesystem, const char* resource_name, const char* path_id = nullptr, bool sortKeys = false, bool bAllowEmptyString = false);

    // Read from a buffer...  Note that the buffer must be null terminated
    bool load_from_buffer(char const* resourceName, const char* pBuffer, void* pFileSystem = nullptr, const char* pPathID = nullptr);

    // Read from a utlbuffer...
    bool load_from_buffer(char const* resourceName, void* buf, void* pFileSystem = nullptr, const char* pPathID = nullptr);

    // Find a keyValue, create it if it is not found.
    // Set bCreate to true to create the key if it doesn't already exist (which ensures a valid pointer will be returned)
    key_values* find_key(const char* keyName, bool bCreate = false);
    key_values* find_key(int keySymbol) const;
    key_values* create_new_key();            // creates a new key, with an autogenerated name.  name is guaranteed to be an integer, of
                                             // value 1 higher than the highest other integer key name
    void add_sub_key(key_values* pSubkey);   // Adds a subkey. Make sure the subkey isn't a child of some other keyvalues
    void remove_sub_key(key_values* subKey); // removes a subkey from the list, DOES NOT DELETE IT

    key_values* get_first_sub_key()
    {
        return sub;
    } // returns the first subkey in the list
    key_values* get_next_key()
    {
        return peer;
    } // returns the next subkey
    void set_next_key(key_values* pDat);
    key_values* find_last_sub_key(); // returns the LAST subkey in the list.  This requires a linked list iteration to find the
                                     // key.  Returns nullptr if we don't have any children

    key_values* get_first_true_sub_key();
    key_values* get_next_true_sub_key();

    key_values* get_first_value(); // When you get a value back, you can use GetX and pass in nullptr to get the value.
    key_values* get_next_value();

    // Data access
    int get_int(const char* keyName = nullptr, int defaultValue = 0);
    uint64_t get_uint64(const char* keyName = nullptr, uint64_t defaultValue = 0);
    float get_float(const char* keyName = nullptr, float defaultValue = 0.0f);
    const char* get_string(const char* keyName = nullptr, const char* defaultValue = "");
    const wchar_t* wet_wstring(const char* keyName = nullptr, const wchar_t* defaultValue = L"");
    void* get_ptr(const char* keyName = nullptr, void* defaultValue = (void*)0);
    bool get_bool(const char* keyName = nullptr, bool defaultValue = false);
    Color get_color(const char* keyName = nullptr /* default value is all black */);
    bool is_empty(const char* keyName = nullptr);

    // Data access
    int get_int(int keySymbol, int defaultValue = 0);
    float get_float(int keySymbol, float defaultValue = 0.0f);
    const char* get_string(int keySymbol, const char* defaultValue = "");
    const wchar_t* get_w_string(int keySymbol, const wchar_t* defaultValue = L"");
    void* get_ptr(int keySymbol, void* defaultValue = (void*)0);
    Color get_color(int keySymbol /* default value is all black */);
    bool is_empty(int keySymbol);

    // Key writing
    void set_w_string(const char* keyName, const wchar_t* value);
    void set_string(const char* keyName, const char* value);
    void set_int(const char* keyName, int value);
    void set_uint_64(const char* keyName, uint64_t value);
    void set_float(const char* keyName, float value);
    void set_ptr(const char* keyName, void* value);
    void set_color(const char* keyName, Color value);
    void set_bool(const char* keyName, bool value)
    {
        set_int(keyName, value ? 1 : 0);
    }

    // Adds a chain... if we don't find stuff in this keyvalue, we'll look
    // in the one we're chained to.
    void chain_key_value(key_values* pChain);

    void recursive_vave_to_file(void* buf, int indentLevel, bool sortKeys = false, bool bAllowEmptyString = false);

    bool write_as_binary(void* buffer);
    bool read_as_binary(void* buffer, int nStackDepth = 0);

    // Allocate & create a new copy of the keys
    key_values* make_copy(void) const;

    // Make a new copy of all subkeys, add them all to the passed-in keyvalues
    void copy_subkeys(key_values* pParent) const;

    // Clear out all subkeys, and the current value
    void clear(void);

    // Data type
    enum types_t
    {
        TYPE_NONE = 0,
        TYPE_STRING,
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_PTR,
        TYPE_WSTRING,
        TYPE_COLOR,
        TYPE_UINT64,
        TYPE_NUMTYPES,
    };
    types_t get_data_type(const char* keyName = nullptr);

    // Virtual deletion function - ensures that KeyValues object is deleted from correct heap
    void delete_this();

    void set_string_value(char const* strValue);

    // unpack a key values list into a structure
    void unpack_into_structure(struct KeyValuesUnpackStructure const* pUnpackTable, void* pDest, size_t DestSizeInBytes);

    // Process conditional keys for widescreen support.
    bool process_resolution_keys(const char* pResString);

    // Dump keyvalues recursively into a dump context
    bool dump(class key_values_dump_context* pDump, int nIndentLevel = 0);

    // Merge in another KeyValues, keeping "our" settings
    void recursive_merge_key_values(key_values* baseKV);

private:
    key_values(key_values&); // prevent copy constructor being used

    // prevent delete being called except through deleteThis()
    ~key_values();

    key_values* create_key(const char* keyName);

    /// Create a child key, given that we know which child is currently the last child.
    /// This avoids the O(N^2) behaviour when adding children in sequence to KV,
    /// when create_key() wil have to re-locate the end of the list each time.  This happens,
    /// for example, every time we load any KV file whatsoever.
    key_values* create_key_using_known_last_child(const char* keyName, key_values* pLastChild);
    void add_subkey_using_known_last_child(key_values* pSubKey, key_values* pLastChild);

    void recursive_copy_key_values(key_values& src);
    void remove_everything();
    //	void RecursiveSaveToFile( IBaseFileSystem *filesystem, void*buffer, int indentLevel );
    //	void WriteConvertedString( void*buffer, const char *pszString );

    // NOTE: If both filesystem and pBuf are non-null, it'll save to both of them.
    // If filesystem is null, it'll ignore f.
    void recursive_save_to_file(void* filesystem, file_handle_t f, void* pBuf, int indentLevel, bool sortKeys, bool bAllowEmptyString);
    void save_key_to_file(key_values* dat, void* filesystem, file_handle_t f, void* pBuf, int indentLevel, bool sortKeys, bool bAllowEmptyString);
    void write_converted_string(void* filesystem, file_handle_t f, void* pBuf, const char* pszString);

    void recursive_load_from_buffer(char const* resourceName, void* buf);

    // For handling #include "filename"
    void append_included_keys(void* includedKeys);
    void parse_included_keys(char const* resourceName, const char* filetoinclude, void* pFileSystem, const char* pPathID, void* includedKeys);

    // For handling #base "filename"
    void merge_base_keys(void* baseKeys);

    // NOTE: If both filesystem and pBuf are non-null, it'll save to both of them.
    // If filesystem is null, it'll ignore f.
    void internal_write(void* filesystem, file_handle_t f, void* pBuf, const void* pData, int len);

    void init();
    const char* read_token(void* buf, bool& wasQuoted, bool& wasConditional);
    void write_indents(void* filesystem, file_handle_t f, void* pBuf, int indentLevel);

    void free_allocated_value();
    void allocate_value_block(int size);

    int key_name; // keyname is a symbol defined in KeyValuesSystem

    // These are needed out of the union because the API returns string pointers
    char* s_value;
    wchar_t* ws_value;

    // we don't delete these
    union
    {
        int i_alue;
        float fl_value;
        void* p_value;
        unsigned char color[4];
    };

    char data_type;
    char has_escape_sequences;  // true, if while parsing this KeyValue, Escape Sequences are used (default false)
    char evaluate_conditionals; // true, if while parsing this KeyValue, conditionals blocks are evaluated (default true)
    char unused[1];

    key_values* peer;  // pointer to next key in list
    key_values* sub;   // pointer to Start of a new sub key list
    key_values* chain; // Search here if it's not in our list

private:
    static int (*get_symbol_for_string)(const char* name, bool bCreate);
    static const char* (*get_string_for_symbol)(int symbol);
    static void* s_pGrowableStringTable;

public:
    static int get_symbol_for_string_classic(const char* name, bool bCreate = true);
    static const char* get_string_for_symbol_classic(int symbol);

    static int get_symbol_for_string_growable(const char* name, bool bCreate = true);
    static const char* get_string_for_symbol_growable(int symbol);

    static int call_get_symbol_for_string(const char* name, bool bCreate = true)
    {
        return get_symbol_for_string(name, bCreate);
    }
    static const char* call_get_string_for_symbol(int symbol)
    {
        return get_string_for_symbol(symbol);
    }
};