/*
Stefan Millar
0996703
smilla07
Assignment 2
*/
#include "SVGParser.h"
#include <math.h>

// A2 Functions
//ToJSON Functions
char* attrToJSON(const Attribute *a)
{
    if(a == NULL)
    {
        char *empty = malloc(sizeof(char) * 3);
        strcpy(empty, "{}");
        return empty;
    }

    int size = strlen(a->name) + strlen(a->value) + strlen("{\"name\":\"\",\"value\":\"\"}") + 1;
    char *json = malloc(size);

    snprintf(json, size, "{\"name\":\"%s\",\"value\":\"%s\"}", a->name, a->value);

    return json;
}

char* circleToJSON(const Circle *c)
{
    if(c == NULL)
    {
        char *empty = malloc(sizeof(char) * 3);
        strcpy(empty, "{}");
        return empty;
    }

    char temp1[15];
    char temp2[15];
    char temp3[15];
    char temp4[15];
    sprintf(temp1, "%.2f", c->cx);
    sprintf(temp2, "%.2f", c->cy);
    sprintf(temp3, "%.2f", c->r);
    sprintf(temp4, "%d", getLength(c->otherAttributes));

    int size = strlen(temp1) + strlen(temp2) + strlen(temp3) + strlen(temp4) + strlen(c->units)
     + strlen("{\"cx\":,\"cy\":,\"r\":,\"numAttr\":,\"units\":\"\"}") + 1;
    char *json = malloc(size);

    snprintf(json, size, "{\"cx\":%s,\"cy\":%s,\"r\":%s,\"numAttr\":%s,\"units\":\"%s\"}",
    temp1, temp2, temp3, temp4, c->units);

    return json;
}

char* rectToJSON(const Rectangle *r)
{
    if(r == NULL)
    {
        char *empty = malloc(sizeof(char) * 3);
        strcpy(empty, "{}");
        return empty;
    }

    char temp1[15];
    char temp2[15];
    char temp3[15];
    char temp4[15];
    char temp5[15];
    sprintf(temp1, "%.2f", r->x);
    sprintf(temp2, "%.2f", r->y);
    sprintf(temp3, "%.2f", r->width);
    sprintf(temp4, "%.2f", r->height);
    sprintf(temp5, "%d", getLength(r->otherAttributes));

    int size = strlen(temp1) + strlen(temp2) + strlen(temp3) + strlen(temp4) + strlen(temp5) + strlen(r->units)
     + strlen("{\"x\":,\"y\":,\"w\":,\"h\":,\"numAttr\":,\"units\":\"\"}") + 1;
    char *json = malloc(size);

    snprintf(json, size, "{\"x\":%s,\"y\":%s,\"w\":%s,\"h\":%s,\"numAttr\":%s,\"units\":\"%s\"}",
    temp1, temp2, temp3, temp4, temp5, r->units);

    return json;
}

char* pathToJSON(const Path *p)
{
    if(p == NULL)
    {
        char *empty = malloc(sizeof(char) * 3);
        strcpy(empty, "{}");
        return empty;
    }
    char temp1[15];
    char data[65];
    strncpy(data, p->data, 64);
    data[64] = '\0';
    sprintf(temp1, "%d", getLength(p->otherAttributes));

    int size = strlen(data) + strlen(temp1) + strlen("{\"d\":\"\",\"numAttr\":}") + 1;
    char *json = malloc(size);

    snprintf(json, size, "{\"d\":\"%s\",\"numAttr\":%s}", data, temp1);

    return json;
}

char* groupToJSON(const Group *g)
{
    if(g == NULL)
    {
        char *empty = malloc(sizeof(char) * 3);
        strcpy(empty, "{}");
        return empty;
    }

    char temp1[15];
    sprintf(temp1, "%d", getLength(g->otherAttributes));
    char temp2[15];
    sprintf(temp2, "%d", getLength(g->rectangles) + getLength(g->circles) + getLength(g->paths) + getLength(g->groups));

    int size = strlen(temp1) + strlen(temp2) + strlen("{\"children\":,\"numAttr\":}") + 1;
    char *json = malloc(size);

    snprintf(json, size, "{\"children\":%s,\"numAttr\":%s}", temp2, temp1);

    return json;
}

char* attrListToJSON(const List *list)
{
    if((list == NULL) || (getLength((List *)list) == 0))
    {
        char *empty = malloc(sizeof(char) * 3);
        strcpy(empty, "[]");
        return empty;
    }

    ListIterator iter = createIterator((List *)list);
    Attribute *attr = nextElement(&iter);
    int size = strlen("[") + 1;
    char *JSON = malloc(size);
    strcpy(JSON, "[");

    //Concat all attributes
    while(attr)
    {
        char *tempJSON = attrToJSON(attr);
        size = size + strlen(tempJSON) + 1;
        JSON = realloc(JSON, size);
        strcat(JSON, tempJSON);
        strcat(JSON, ",");
        free(tempJSON);
        attr = nextElement(&iter);
    }
    JSON[size - 2] = ']';

    return JSON;
}

char* circListToJSON(const List *list)
{
    if((list == NULL) || (getLength((List *)list) == 0))
    {
        char *empty = malloc(sizeof(char) * 3);
        strcpy(empty, "[]");
        return empty;
    }

    ListIterator iter = createIterator((List *)list);
    Circle *circ = nextElement(&iter);
    int size = strlen("[") + 1;
    char *JSON = malloc(size);
    strcpy(JSON, "[");

    //Concat all circles
    while(circ)
    {
        char *tempJSON = circleToJSON(circ);
        size = size + strlen(tempJSON) + 1;
        JSON = realloc(JSON, size);
        strcat(JSON, tempJSON);
        strcat(JSON, ",");
        free(tempJSON);
        circ = nextElement(&iter);
    }
    JSON[size - 2] = ']';

    return JSON;
}

char* rectListToJSON(const List *list)
{
    if((list == NULL) || (getLength((List *)list) == 0))
    {
        char *empty = malloc(sizeof(char) * 3);
        strcpy(empty, "[]");
        return empty;
    }

    ListIterator iter = createIterator((List *)list);
    Rectangle *rect = nextElement(&iter);
    int size = strlen("[") + 1;
    char *JSON = malloc(size);
    strcpy(JSON, "[");

    //Concat all rectangles
    while(rect)
    {
        char *tempJSON = rectToJSON(rect);
        size = size + strlen(tempJSON) + 1;
        JSON = realloc(JSON, size);
        strcat(JSON, tempJSON);
        strcat(JSON, ",");
        free(tempJSON);
        rect = nextElement(&iter);
    }
    JSON[size - 2] = ']';

    return JSON;
}

char* pathListToJSON(const List *list)
{
    if((list == NULL) || (getLength((List *)list) == 0))
    {
        char *empty = malloc(sizeof(char) * 3);
        strcpy(empty, "[]");
        return empty;
    }

    ListIterator iter = createIterator((List *)list);
    Path *path = nextElement(&iter);
    int size = strlen("[") + 1;
    char *JSON = malloc(size);
    strcpy(JSON, "[");

    //Concat all paths
    while(path)
    {
        char *tempJSON = pathToJSON(path);
        size = size + strlen(tempJSON) + 1;
        JSON = realloc(JSON, size);
        strcat(JSON, tempJSON);
        strcat(JSON, ",");
        free(tempJSON);
        path = nextElement(&iter);
    }
    JSON[size - 2] = ']';

    return JSON;
}

char* groupListToJSON(const List *list)
{
    if((list == NULL) || (getLength((List *)list) == 0))
    {
        char *empty = malloc(sizeof(char) * 3);
        strcpy(empty, "[]");
        return empty;
    }

    ListIterator iter = createIterator((List *)list);
    Group *group = nextElement(&iter);
    int size = strlen("[") + 1;
    char *JSON = malloc(size);
    strcpy(JSON, "[");

    //Concat all groups
    while(group)
    {
        char *tempJSON = groupToJSON(group);
        size = size + strlen(tempJSON) + 1;
        JSON = realloc(JSON, size);
        strcat(JSON, tempJSON);
        strcat(JSON, ",");
        free(tempJSON);
        group = nextElement(&iter);
    }
    JSON[size - 2] = ']';

    return JSON;
}

char* SVGtoJSON(const SVGimage* image)
{
    if(image == NULL)
    {
        char *empty = malloc(sizeof(char) * 3);
        strcpy(empty, "{}");
        return empty;
    }
    List *rectList = getRects((SVGimage *)image);
    List *circList = getCircles((SVGimage *)image);
    List *pathList = getPaths((SVGimage *)image);
    List *groupList = getGroups((SVGimage *)image);

    char temp1[15];
    sprintf(temp1, "%d", getLength(rectList));
    char temp2[15];
    sprintf(temp2, "%d", getLength(circList));
    char temp3[15];
    sprintf(temp3, "%d", getLength(pathList));
    char temp4[15];
    sprintf(temp4, "%d", getLength(groupList));

    int size = strlen(temp1) + strlen(temp2) + strlen(temp3) + strlen(temp4)
     + strlen("{\"numRect\":,\"numCirc\":,\"numPaths\":,\"numGroups\":}") + 1;
    char *json = malloc(size);

    snprintf(json, size, "{\"numRect\":%s,\"numCirc\":%s,\"numPaths\":%s,\"numGroups\":%s}", temp1, temp2, temp3, temp4);

    freeTheList(rectList);
    freeTheList(circList);
    freeTheList(pathList);
    freeTheList(groupList);
    return json;
}

//Module 2
void setAttribute(SVGimage* image, elementType elemType, int elemIndex, Attribute* newAttribute)
{
    if(image == NULL)
        return;
    if(newAttribute == NULL)
        return;
    if(newAttribute->name == NULL)
        return;
    if(newAttribute->value == NULL)
        return;
    if((elemIndex < 0) && (elemType != SVG_IMAGE))
        return;

    if(elemType == RECT)
        setRectAttribute(image, elemIndex, newAttribute);
    else if(elemType == CIRC)
        setCircAttribute(image, elemIndex, newAttribute);
    else if(elemType == PATH)
        setPathAttribute(image, elemIndex, newAttribute);
    else if(elemType == GROUP)
        setGroupAttribute(image, elemIndex, newAttribute);
    else if(elemType == SVG_IMAGE)
        setOtherAttribute(image->otherAttributes, newAttribute);

}

void addComponent(SVGimage* image, elementType type, void* newElement)
{
    if(image == NULL)
        return;
    if(newElement == NULL)
        return;

    if(type == RECT)
        insertBack(image->rectangles, newElement);
    else if(type == CIRC)
        insertBack(image->circles, newElement);
    else if(type == PATH)
        insertBack(image->paths, newElement);
}

//Module1
bool validateSVGimage(SVGimage* image, char* schemaFile)
{
    if(image == NULL)
        return false;
    if(schemaFile == NULL)
        return false;

    int isValid = false;
    xmlSchemaParserCtxt *parser = NULL;
    xmlSchemaValidCtxt *schemaPtr = NULL;
    xmlSchema *schema = NULL;

    parser = xmlSchemaNewParserCtxt(schemaFile);
    schema = xmlSchemaParse(parser);
    schemaPtr = xmlSchemaNewValidCtxt(schema);
    xmlDoc *doc = createXMLdoc(image);
    isValid = xmlSchemaValidateDoc(schemaPtr, doc);

    if(schema != NULL)
        xmlSchemaFree(schema);
    if(schemaPtr != NULL)
        xmlSchemaFreeValidCtxt(schemaPtr);

    xmlSchemaFreeParserCtxt(parser);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    if(isValid == 0)
        return true;
    else
        return false;
}

SVGimage* createValidSVGimage(char* fileName, char* schemaFile)
{
    if(fileName == NULL)
        return false;
    if(schemaFile == NULL)
        return false;

    xmlDoc *doc = NULL;
    SVGimage *image = NULL;
    int isValid = false;
    xmlSchemaParserCtxt *parser = NULL;
    xmlSchemaValidCtxt *schemaPtr = NULL;
    xmlSchema *schema = NULL;

    doc = xmlReadFile(fileName, NULL, 0);
    parser = xmlSchemaNewParserCtxt(schemaFile);
    schema = xmlSchemaParse(parser);
    schemaPtr = xmlSchemaNewValidCtxt(schema);
    isValid = xmlSchemaValidateDoc(schemaPtr, doc);

    if(schema != NULL)
        xmlSchemaFree(schema);
    if(schemaPtr != NULL)
        xmlSchemaFreeValidCtxt(schemaPtr);

    xmlSchemaFreeParserCtxt(parser);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    if(isValid == 0)
    {
        image = createSVGimage(fileName);
        if(validateSVGimage(image, schemaFile) == true)
            return image;
        else
            return NULL;
    }
    else
        return NULL;
}

bool writeSVGimage(SVGimage* image, char* fileName)
{
    if(image == NULL)
        return false;
    if(fileName == NULL)
        return NULL;

    xmlDoc *doc = createXMLdoc(image);
    int isValid = xmlSaveFile(fileName, doc);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    if(isValid < 0)
        return false;
    else
        return true;
}

//HELPERS
void setRectAttribute(SVGimage* image, int index, Attribute* newAttribute)
{
    if(index > (getLength(image->rectangles) - 1))
        return;

    ListIterator iter = createIterator(image->rectangles);
    Rectangle *rect;
    int i;

    for(i = 0; i <= index; i++)
        rect = (Rectangle *)nextElement(&iter);

    if(strcmp(newAttribute->name, "width") == 0)
    {
        rect->width = stringToFloat(newAttribute->value);
        deleteAttribute((void *)newAttribute);
    }
    else if(strcmp(newAttribute->name, "height") == 0)
    {
        rect->height = stringToFloat(newAttribute->value);
        deleteAttribute((void *)newAttribute);
    }
    else if(strcmp(newAttribute->name, "x") == 0)
    {
        rect->x = stringToFloat(newAttribute->value);
        deleteAttribute((void *)newAttribute);
    }
    else if(strcmp(newAttribute->name, "y") == 0)
    {
        rect->y = stringToFloat(newAttribute->value);
        deleteAttribute((void *)newAttribute);
    }
    else
        setOtherAttribute(rect->otherAttributes, newAttribute);
}

void setCircAttribute(SVGimage* image, int index, Attribute* newAttribute)
{
    if(index > (getLength(image->circles) - 1))
        return;
    
    ListIterator iter = createIterator(image->circles);
    Circle *circ;
    int i;

    for(i = 0; i <= index; i++)
        circ = (Circle *)nextElement(&iter);

    if(strcmp(newAttribute->name, "cx") == 0)
    {
        circ->cx = stringToFloat(newAttribute->value);
        deleteAttribute((void *)newAttribute);
    }
    else if(strcmp(newAttribute->name, "cy") == 0)
    {
        circ->cy = stringToFloat(newAttribute->value);
        deleteAttribute((void *)newAttribute);
    }
    else if(strcmp(newAttribute->name, "r") == 0)
    {
        circ->r = stringToFloat(newAttribute->value);
        deleteAttribute((void *)newAttribute);
    }
    else
        setOtherAttribute(circ->otherAttributes, newAttribute);
}

void setPathAttribute(SVGimage* image, int index, Attribute* newAttribute)
{
    if(index > (getLength(image->paths) - 1))
        return;
    
    ListIterator iter = createIterator(image->paths);
    Path *path;
    int i;

    for(i = 0; i <= index; i++)
        path = (Path *)nextElement(&iter);

    if(strcmp(newAttribute->name, "d") == 0)
    {
        free(path->data);
        path->data = malloc(sizeof(char) * strlen(newAttribute->value) + 1);
        strcpy(path->data, newAttribute->value);
        deleteAttribute((void *)newAttribute);
    }
    else
        setOtherAttribute(path->otherAttributes, newAttribute);
}

void setGroupAttribute(SVGimage* image, int index, Attribute* newAttribute)
{
    if(index > (getLength(image->groups) - 1))
        return;
    
    ListIterator iter = createIterator(image->groups);
    Group *group;
    int i;

    for(i = 0; i <= index; i++)
        group = (Group *)nextElement(&iter);

    setOtherAttribute(group->otherAttributes, newAttribute);
}

void setOtherAttribute(List *otherAttributes, Attribute *attr)
{
    ListIterator iter = createIterator(otherAttributes);
    Attribute *attributes = (Attribute *)nextElement(&iter);

    while(attributes)
    {
        if(strcmp(attributes->name, attr->name) == 0)
        {
            free(attributes->value);
            attributes->value = malloc(sizeof(char) * strlen(attr->value) + 1);
            strcpy(attributes->value, attr->value);
            deleteAttribute((void *)attr);
            return;
        }
        attributes = (Attribute *)nextElement(&iter);
    }
    Attribute *temp = malloc(sizeof(Attribute));
    temp->name = malloc(sizeof(char) * strlen(attr->name) + 1);
    temp->value = malloc(sizeof(char) * strlen(attr->value) + 1);
    strcpy(temp->name, attr->name);
    strcpy(temp->value, attr->value);
    deleteAttribute((void *)attr);

    insertBack(otherAttributes, (void *)(temp));
}

xmlDoc* createXMLdoc(SVGimage *image)
{
    if(image == NULL)
        return NULL;

    bool isValidSvg = true;
    xmlDoc* doc = NULL;
    xmlNode* root = NULL;

    // Create doc
    doc = xmlNewDoc(NULL);

    //Set root
    root = xmlNewNode(NULL, BAD_CAST "svg");
    xmlNs *ns = xmlNewNs(root, BAD_CAST image->namespace, NULL);
    xmlSetNs(root, ns);
    isValidSvg = setRootChildren(&root, image);
    xmlDocSetRootElement(doc, root);

    if(isValidSvg == false)
    {
        xmlFreeDoc(doc);
        return NULL;
    }
    else
        return doc;
}

bool setRootChildren(xmlNode **root, SVGimage *image)
{
    if(strcmp(image->title, "") != 0)
    {
        char name[256];
        strncpy(name, image->title, 255);
        xmlNewChild(*root, NULL, BAD_CAST "title", BAD_CAST name);
    }
    if(strcmp(image->description, "") != 0)
    {
        char name[256];
        strncpy(name, image->description, 255);
        xmlNewChild(*root, NULL, BAD_CAST "desc", BAD_CAST name);
    }
    if(getLength(image->rectangles) > 0)
    {
        if(setXMLRectangles(root, image->rectangles) == false)
            return false;
    }
    if(getLength(image->circles) > 0)
    {
        if(setXMLCircles(root, image->circles) == false)
            return false;
    }
    if(getLength(image->paths) > 0)
    {
        if(setXMLPaths(root, image->paths) == false)
            return false;
    }
    if(getLength(image->groups) > 0)
    {
        if(setXMLGroups(root, image->groups) == false)
            return false;
    }
    
    if(setXMLAttributes(root, image->otherAttributes) == false)
        return false;

    return true;
}

bool setXMLRectangles(xmlNode** parent, List *rectangles)
{
    ListIterator iter = createIterator(rectangles);
    Rectangle *data = nextElement(&iter);
    if(data == NULL)
        return false;

    while(data)
    {
        //Check validaty of rectangle
        if(data->width < 0)
            return false;
        if(data->height < 0)
            return false;
        if(data->otherAttributes == NULL)
            return false;
        
        xmlNode *tempNode = xmlNewChild(*parent, NULL, BAD_CAST "rect", NULL);
        char buffer[100];

        snprintf(buffer, 99, "%f%s", data->x, data->units);
        xmlNewProp(tempNode, BAD_CAST "x", BAD_CAST buffer);

        memset(buffer, 0, 99);
        snprintf(buffer, 99, "%f%s", data->y, data->units);
        xmlNewProp(tempNode, BAD_CAST "y", BAD_CAST buffer);

        memset(buffer, 0, 99);
        snprintf(buffer, 99, "%f%s", data->width, data->units);
        xmlNewProp(tempNode, BAD_CAST "width", BAD_CAST buffer);

        memset(buffer, 0, 99);
        snprintf(buffer, 99, "%f%s", data->height, data->units);
        xmlNewProp(tempNode, BAD_CAST "height", BAD_CAST buffer);

        if(setXMLAttributes(&tempNode, data->otherAttributes) == false)
            return false;

        data = nextElement(&iter);
    }
    return true;
}

bool setXMLCircles(xmlNode** parent, List *circles)
{
    ListIterator iter = createIterator(circles);
    Circle *data = nextElement(&iter);
    if(data == NULL)
        return false;

    while(data)
    {
        //Check validaty of circle
        if(data->r < 0)
            return false;
        if(data->otherAttributes == NULL)
            return false;

        xmlNode *tempNode = xmlNewChild(*parent, NULL, BAD_CAST "circle", NULL);
        char buffer[100];

        snprintf(buffer, 99, "%f%s", data->cx, data->units);
        xmlNewProp(tempNode, BAD_CAST "cx", BAD_CAST buffer);

        memset(buffer, 0, 99);
        snprintf(buffer, 99, "%f%s", data->cy, data->units);
        xmlNewProp(tempNode, BAD_CAST "cy", BAD_CAST buffer);

        memset(buffer, 0, 99);
        snprintf(buffer, 99, "%f%s", data->r, data->units);
        xmlNewProp(tempNode, BAD_CAST "r", BAD_CAST buffer);

        if(setXMLAttributes(&tempNode, data->otherAttributes) == false)
            return false;

        data = nextElement(&iter);
    }
    return true;
}

bool setXMLPaths(xmlNode** parent, List *paths)
{
    ListIterator iter = createIterator(paths);
    Path *data = nextElement(&iter);

    if(data == NULL)
        return false;

    while(data)
    {
        //Check validaty of path
        if(data->data == NULL)
            return false;
        if(data->otherAttributes == NULL)
            return false;

        xmlNode *tempNode = xmlNewChild(*parent, NULL, BAD_CAST "path", NULL);
        xmlNewProp(tempNode, BAD_CAST "d", BAD_CAST data->data);

        if(setXMLAttributes(&tempNode, data->otherAttributes) == false)
            return false;

        data = nextElement(&iter);
    }
    return true;
}

bool setXMLGroups(xmlNode** parent, List *groups)
{
    ListIterator iter = createIterator(groups);
    Group *data = nextElement(&iter);

    if(data == NULL)
        return false;
    
    while(data)
    {
        //Check validaty of groups
        if(data->rectangles == NULL)
            return false;
        if(data->circles == NULL)
            return false;
        if(data->paths == NULL)
            return false;
        if(data->groups == NULL)
            return false;
        if(data->otherAttributes == NULL)
            return false;

        xmlNode *tempNode = xmlNewChild(*parent, NULL, BAD_CAST "g", NULL);
        
        if(getLength(data->rectangles) > 0)
            setXMLRectangles(&tempNode, data->rectangles);
        if(getLength(data->circles) > 0)
            setXMLCircles(&tempNode, data->circles);
        if(getLength(data->paths) > 0)
            setXMLPaths(&tempNode, data->paths);
        if(getLength(data->groups) > 0)
            setXMLGroups(&tempNode, data->groups);
    
        if(setXMLAttributes(&tempNode, data->otherAttributes) == false)
            return false;

        data = nextElement(&iter);
    }
    return true;
}

bool setXMLAttributes(xmlNode **node, List *attributes)
{
    if(attributes == NULL)
        return false;

    if(getLength(attributes) > 0)
    {
        ListIterator iter = createIterator(attributes);
        Attribute *attr = nextElement(&iter);
        if(attr == NULL)
            return false;

        while(attr)
        {
            //Check validaty of attributes
            if(attr->name == NULL)
                return false;
            if(attr->value == NULL)
                return false;

            xmlNewProp(*node, BAD_CAST attr->name, BAD_CAST attr->value);
            attr = nextElement(&iter);
        }
    }
    return true;
}

//A1 functions
SVGimage* createSVGimage(char* fileName)
{
    if(fileName == NULL)
        return NULL;

    xmlDoc *doc = NULL;
    xmlNode *root = NULL;
    SVGimage *image = malloc(sizeof(SVGimage));

    //Read in xml file
    doc = xmlReadFile(fileName, NULL, 0);

    if(doc == NULL)
    {
        free(image);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return NULL;
    }


    //Get root node
    root = xmlDocGetRootElement(doc);

    //Initialize SVGimage
    strcpy(image->title, "");
    strcpy(image->description, "");
    strcpy(image->namespace, "");
    image->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    image->circles = initializeList(circleToString, deleteCircle, compareCircles);
    image->paths = initializeList(pathToString, deletePath, comparePaths);
    image->groups = initializeList(groupToString, deleteGroup, compareGroups);
    image->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    //Get SVGImage name
    if(strlen((char *)(root->ns->href)) < 256)
        strcpy(image->namespace, (char *)(root->ns->href));

    setAttributes(&(image->otherAttributes), root);
    xmlNode *list = root->children;

    while(list)
    {
        if(strcmp((char *)(list->name), "rect") == 0)
            setRectangle(&(image->rectangles), list);
        else if(strcmp((char *)(list->name), "circle") == 0)
            setCircle(&(image->circles), list);
        else if(strcmp((char *)(list->name), "path") == 0)
            setPath(&(image->paths), list);
        else if(strcmp((char *)(list->name), "g") == 0)
            setGroup(&(image->groups), list);
        else if(strcmp((char *)(list->name), "title") == 0)
        {
            strncpy(image->title, (char *)(list->children->content), 255);
            (image->title)[255] = ' ';
        }
        else if(strcmp((char *)(list->name), "desc") == 0)
        {
            strncpy(image->description, (char *)(list->children->content), 255);
            (image->description)[255] = ' ';
        }

        list = list->next;
    }

    //Free xml Doc
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return image;
}

char* SVGimageToString(SVGimage* img)
{
    if(img == NULL)
        return NULL;

    char *temp1, *temp2, *temp3, *temp4, *temp5;
    temp1 = toString(img->otherAttributes);
    temp2 = toString(img->rectangles);
    temp3 = toString(img->circles);
    temp4 = toString(img->paths);
    temp5 = toString(img->groups);

    //Get memory size
    int size = strlen(img->namespace) + strlen(temp1)
    + strlen(temp2) + strlen(temp3)
    + strlen(temp4) + strlen(temp5)
    + strlen(img->title) + strlen(img->description)
    + strlen("SVG\nnamespace:  title:  description: ") + 1;
    char *SVGstring = malloc(size);
    strcpy(SVGstring, "SVG\nnamespace: ");

    //Concat namespace/description/title
    strcat(SVGstring, img->namespace);
    strcat(SVGstring, " title: ");
    strcat(SVGstring, img->title);
    strcat(SVGstring, " description: ");
    strcat(SVGstring, img->description);

    //Concat all lists and free all lists
    strcat(SVGstring, temp1);
    free(temp1);
    strcat(SVGstring, temp2);
    free(temp2);
    strcat(SVGstring, temp3);
    free(temp3);
    strcat(SVGstring, temp4);
    free(temp4);
    strcat(SVGstring, temp5);
    free(temp5);
    
    return SVGstring;
}

void deleteSVGimage(SVGimage* img)
{
    if(img)
    {
        freeList(img->otherAttributes);
        freeList(img->rectangles);
        freeList(img->circles);
        freeList(img->paths);
        freeList(img->groups);
        free(img);
    }
}

// ***SET FUNCTIONS***
// ATTRIBUTES
void setAttributes(List** otherAttributes, xmlNode *root)
{
    xmlAttr *attr;

    for(attr = root->properties; attr != NULL; attr = attr->next)
    {
        xmlNode *value = attr->children;
        insertBack(*otherAttributes, getAttribute(value, attr));
    }
    (*otherAttributes)->length = getLength(*otherAttributes);
}

void* getAttribute(xmlNode *value, xmlAttr *attr)
{
    Attribute *tempAttribute = malloc(sizeof(Attribute));

    //Copy data into buffer
    tempAttribute->name = malloc(strlen((char *)(attr->name)) + 1);
    strcpy(tempAttribute->name, (char *)attr->name);

    if(value)
    {
        tempAttribute->value = malloc(strlen((char *)(value->content)) + 1);
        strcpy(tempAttribute->value, (char *)(value->content));
    }
    else
    {
        free(tempAttribute->name);
        free(tempAttribute);
        return NULL;
    }

    return (void *)(tempAttribute);
}

void setRectangle(List** rect, xmlNode *data)
{
    xmlAttr *attr;

    //Initialize Rectangle
    attr = data->properties;
    Rectangle *tempRectangle = malloc(sizeof(Rectangle));
    tempRectangle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    tempRectangle->x = 0;
    tempRectangle->y = 0;
    tempRectangle->width = 0;
    tempRectangle->height = 0;
    strcpy(tempRectangle->units, "");

    for(attr = data->properties; attr != NULL; attr = attr->next)
    {
        xmlNode *value = attr->children;

        //Check for which attribute and assign accordingly
        if(strcmp((char *)(attr->name), "x") == 0)
        {
            char *tempStr = unitsFromString((char *)(value->content));
            tempRectangle->x = stringToFloat((char *)(value->content));
            strcpy(tempRectangle->units, tempStr);
            free(tempStr);
        }
        else if(strcmp((char *)(attr->name), "y") == 0)
            tempRectangle->y = stringToFloat((char *)(value->content));
        else if(strcmp((char *)(attr->name), "width") == 0)
            tempRectangle->width = stringToFloat((char *)(value->content));
        else if(strcmp((char *)(attr->name), "height") == 0)
            tempRectangle->height = stringToFloat((char *)(value->content));
        else
        {
            insertBack(tempRectangle->otherAttributes, getAttribute(value, attr));
        }
    }

    //Set list
    insertBack(*rect, (void *)tempRectangle);
    (*rect)->length = getLength(*rect);
}

void setCircle(List** circ, xmlNode *data)
{
    xmlAttr *attr;

    //Initialize Circle
    attr = data->properties;
    Circle *tempCircle = malloc(sizeof(Circle));
    tempCircle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    tempCircle->cx = 0;
    tempCircle->cy = 0;
    tempCircle->r = 0;
    strcpy(tempCircle->units, "");

    for(attr = data->properties; attr != NULL; attr = attr->next)
    {
        xmlNode *value = attr->children;

        //Check for which attribute and assign accordingly
        if(strcmp((char *)(attr->name), "cx") == 0)
        {
            char *tempStr = unitsFromString((char *)(value->content));
            tempCircle->cx = stringToFloat((char *)(value->content));
            strcpy(tempCircle->units, tempStr);
            free(tempStr);
        }
        else if(strcmp((char *)(attr->name), "cy") == 0)
            tempCircle->cy = stringToFloat((char *)(value->content));
        else if(strcmp((char *)(attr->name), "r") == 0)
            tempCircle->r = stringToFloat((char *)(value->content));
        else
        {
            insertBack(tempCircle->otherAttributes, getAttribute(value, attr));
        }
    }

    //Set list
    insertBack(*circ, (void *)tempCircle);
    (*circ)->length = getLength(*circ);
}

void setPath(List** path, xmlNode *data)
{
    xmlAttr *attr;

    //Initialize Circle
    attr = data->properties;
    Path *tempPath = malloc(sizeof(Path));
    tempPath->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    for(attr = data->properties; attr != NULL; attr = attr->next)
    {
        xmlNode *value = attr->children;

        //Check for which attribute and assign accordingly
        if(strcmp((char *)(attr->name), "d") == 0)
        {
            tempPath->data = malloc(strlen((char *)(value->content)) + 1);
            strcpy(tempPath->data, (char *)(value->content));
        }
        else
            insertBack(tempPath->otherAttributes, getAttribute(value, attr));
    }

    //Set list
    insertBack(*path, (void *)tempPath);
    (*path)->length = getLength(*path);
}

void setGroup(List **groupList, xmlNode *data)
{
    Group *theGroup = malloc(sizeof(Group));
    theGroup->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    theGroup->circles = initializeList(circleToString, deleteCircle, compareCircles);
    theGroup->paths = initializeList(pathToString, deletePath, comparePaths);
    theGroup->groups = initializeList(groupToString, deleteGroup, compareGroups);
    theGroup->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    xmlNode *data2 = data->children;
    setAttributes(&(theGroup->otherAttributes), data);

    while(data2)
    {
        if(strcmp((char *)(data2->name), "rect") == 0)
            setRectangle(&(theGroup->rectangles), data2);
        else if(strcmp((char *)(data2->name), "circle") == 0)
            setCircle(&(theGroup->circles), data2);
        else if(strcmp((char *)(data2->name), "path") == 0)
            setPath(&(theGroup->paths), data2);
        else if(strcmp((char *)(data2->name), "g") == 0)
            setGroup(&(theGroup->groups), data2);

        data2 = data2->next;
    }
    insertBack(*groupList, (void *)theGroup);
}

// ***GET FUNCTIONS***
List* getRects(SVGimage* img)
{
    if(img == NULL)
        return NULL;
    
    List* rect = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    Node* temp = img->rectangles->head;

    while(temp)
    {
        insertBack(rect, temp->data);
        temp = temp->next;
    }

    getRectGroups(img->groups, &rect);

    return rect;
}

void getRectGroups(List *group, List** rect)
{
    Node* temp2 = group->head;

    while(temp2)
    {
        Group *tempGroup = (Group *)(temp2->data);
        Node *temp = tempGroup->rectangles->head;
        while(temp)
        {
            insertBack(*rect, temp->data);
            temp = temp->next;
        }
    
        if(tempGroup->groups->head)
            getRectGroups(tempGroup->groups, rect);

        temp2 = temp2->next;
    }
}

List* getCircles(SVGimage* img)
{
    if(img == NULL)
        return NULL;

    List* circ = initializeList(circleToString, deleteCircle, compareCircles);
    Node* temp = img->circles->head;

    while(temp)
    {
        insertBack(circ, temp->data);
        temp = temp->next;
    }

    getCircGroups(img->groups, &circ);

    return circ;
}

void getCircGroups(List *group, List** circ)
{
    Node* temp2 = group->head;

    while(temp2)
    {
        Group *tempGroup = (Group *)(temp2->data);
        Node *temp = tempGroup->circles->head;
        while(temp)
        {
            insertBack(*circ, temp->data);
            temp = temp->next;
        }
    
        if(tempGroup->groups->head)
            getCircGroups(tempGroup->groups, circ);

        temp2 = temp2->next;
    }
}

List* getGroups(SVGimage* img)
{
    if(img == NULL)
        return NULL;

    List* group = initializeList(groupToString, deleteGroup, compareGroups);

    getTheGroups(img->groups, &group);

    return group;
}

void getTheGroups(List *group, List** groups)
{
    Node* temp2 = group->head;

    while(temp2)
    {
        Group *tempGroup = (Group *)(temp2->data);
        insertBack(*groups, temp2->data);
    
        if(tempGroup->groups->head)
            getTheGroups(tempGroup->groups, groups);

        temp2 = temp2->next;
    }
}

List* getPaths(SVGimage* img)
{
    if(img == NULL)
        return NULL;

    List* path = initializeList(pathToString, deletePath, comparePaths);
    Node* temp = img->paths->head;

    while(temp)
    {
        insertBack(path, temp->data);
        temp = temp->next;
    }

    getPathGroups(img->groups, &path);

    return path;
}

void getPathGroups(List *group, List** path)
{
    Node* temp2 = group->head;

    while(temp2)
    {
        Group *tempGroup = (Group *)(temp2->data);
        Node *temp = tempGroup->paths->head;
        while(temp)
        {
            insertBack(*path, temp->data);
            temp = temp->next;
        }
    
        if(tempGroup->groups->head)
            getPathGroups(tempGroup->groups, path);

        temp2 = temp2->next;
    }
}

// ***NUM FUNCTIONS***
int numRectsWithArea(SVGimage* img, float area)
{
    if((img == NULL) || (area < 0))
        return 0;

    int num = 0;
    int rectArea = 0;
    int toCompare = ceil(area);
    List *list = getRects(img);
    Node *temp = list->head;

    while(temp)
    {
        Rectangle *rect = (Rectangle *)temp->data;
        rectArea = ceil(rect->width * rect->height);
        if(rectArea == toCompare)
            num++;

        temp = temp->next;
    }

    freeTheList(list);
    
    return num;
}

int numCirclesWithArea(SVGimage* img, float area)
{
    if((img == NULL) || (area < 0))
        return 0;

    int num = 0;
    int circArea;
    int toCompare = ceil(area);
    List *list = getCircles(img);
    Node *temp = list->head;

    while(temp)
    {
        Circle *circ = (Circle *)temp->data;
        circArea = ceil(circ->r * circ->r * 3.1415926535);
        if(circArea == toCompare)
            num++;

        temp = temp->next;
    }

    freeTheList(list);
    return num;
}

int numPathsWithdata(SVGimage* img, char* data)
{
    if((img == NULL) || (data == NULL))
        return 0;

    int num = 0;
    List *list = getPaths(img);
    Node *temp = list->head;

    while(temp)
    {
        Path *path = (Path *)temp->data;
        if(strcmp(path->data, data) == 0)
            num++;

        temp = temp->next;
    }
    freeTheList(list);
    return num;
}

int numGroupsWithLen(SVGimage* img, int len)
{
    if((img == NULL) || (len < 0))
        return 0;

    int num = 0;
    List *list = getGroups(img);
    Node *temp = list->head;
    int groupLen = 0;

    while(temp)
    {
        Group *group = (Group *)temp->data;
        groupLen = getLength(group->rectangles) + getLength(group->circles) + getLength(group->paths) + getLength(group->groups);
        if(groupLen == len)
            num++;

        temp = temp->next;
    }

    freeTheList(list);
    return num;
}

int numAttr(SVGimage* img)
{
    if(img == NULL)
        return 0;

    int length = 0;

    List* list = getGroups(img);
    Node* temp = list->head;

    //Get groups
    while(temp)
    {
        Group *group = (Group *)temp->data;
        length = length + getLength(group->otherAttributes);
        temp = temp->next;
    }
    freeTheList(list);

    //Get Rectangles
    list = getRects(img);
    temp = list->head;
    while(temp)
    {
        Rectangle *rect = (Rectangle *)temp->data;
        length = length + getLength(rect->otherAttributes);
        temp = temp->next;
    }
    freeTheList(list);

    //Get Circles
    list = getCircles(img);
    temp = list->head;
    while(temp)
    {
        Circle *circ = (Circle *)temp->data;
        length = length + getLength(circ->otherAttributes);
        temp = temp->next;
    }
    freeTheList(list);

    //Get Paths
    list = getPaths(img);
    temp = list->head;
    while(temp)
    {
        Path *path = (Path *)temp->data;
        length = length + getLength(path->otherAttributes);
        temp = temp->next;
    }
    freeTheList(list);

    length = length + getLength(img->otherAttributes);

    return length;
}

// ***HELPER FUNCTIONS***
// ATRRIBUTE
void deleteAttribute( void* data)
{
    Attribute* delete = (Attribute *)data;

    free(delete->name);
    free(delete->value);
    free(delete); 
}

char* attributeToString( void* data)
{
    Attribute *temp = (Attribute *)(data);
    char *string = malloc(strlen("Attribute name:    Attribute value: ") 
    + strlen(temp->name) + strlen(temp->value) + 1);
    strcpy(string, "");
    strcat(string, "Attribute name: ");
    strcat(string, temp->name);
    strcat(string, "   Attribute value: ");
    strcat(string, temp->value);
    return string;
}

int compareAttributes(const void *first, const void *second)
{
    Attribute *cmp1 = (Attribute *)first;
    Attribute *cmp2 = (Attribute *)second;

    if(strcmp(cmp1->name, cmp2->name) != 0)
        return 1;
    if(strcmp(cmp1->value, cmp2->value) != 0)
        return 1;
    else
        return 0;
}

// GROUP
void deleteGroup(void* data)
{
    Group *toFree = (Group *)data;

    freeList(toFree->rectangles);
    freeList(toFree->circles);
    freeList(toFree->paths);
    freeList(toFree->otherAttributes);
    if(getLength(toFree->groups) != 0)
        freeList(toFree->groups);
    else
        free(toFree->groups);
    
    free(data);
}

char* groupToString( void* data)
{
    Group *temp = (Group *)data;
    char *temp1, *temp2, *temp3, *temp4, *temp5;
    temp1 = toString(temp->otherAttributes);
    temp2 = toString(temp->rectangles);
    temp3 = toString(temp->circles);
    temp4 = toString(temp->paths);
    temp5 = toString(temp->groups);
    char *string = malloc(strlen("Group\n") + strlen(temp1)
    + strlen(temp2) + strlen(temp3) + strlen(temp4) + 1);
    strcpy(string, "Group\n");

    strcat(string, temp1);
    strcat(string, temp2);
    strcat(string, temp3);
    strcat(string, temp4);
    free(temp1);
    free(temp2);
    free(temp3);
    free(temp4);
    if(temp->groups->head)
    {
        string = realloc(string, strlen(string) + strlen(temp5) + 1);
        strcat(string, temp5);
    }
    free(temp5);
    return string;
}

int compareGroups(const void *first, const void *second)
{
    return 0;
}

// RECTANGLE
void deleteRectangle(void* data)
{
    Rectangle *toDelete = (Rectangle *)(data);
    freeList(toDelete->otherAttributes);
    free(data);
}

char* rectangleToString(void* data)
{
    Rectangle *temp = (Rectangle *)data;
    char *temp1 = toString(temp->otherAttributes);
    int size = strlen("Rectangle\nX:   Y:   width:   height:   units: \n**OTHER ATTRIBUTES**\n")
    + strlen(temp->units) + strlen(temp1) + 49 * sizeof(char);
    char *string = malloc(size);

    snprintf(string, size - 1, "Rectangle\nX: %f  Y: %f  width: %f  height: %f  units: %s\n**OTHER ATTRIBUTES**%s\n"
    , temp->x, temp->y, temp->width, temp->height, temp->units, temp1);
    free(temp1);

    return string;
}

int compareRectangles(const void *first, const void *second)
{
    Rectangle *cmp1 = (Rectangle *)first;
    Rectangle *cmp2 = (Rectangle *)second;

    if(cmp1->x != cmp2->x)
        return 1;
    else if(cmp1->y != cmp2->y)
        return 1;
    else if(cmp1->width != cmp2->width)
        return 1;
    else if(cmp1->height != cmp2->height)
        return 1;
    else
        return 0;
}

// CIRCLE
void deleteCircle(void* data)
{
    Circle *circle = (Circle *)data;
    freeList(circle->otherAttributes);
    free(data);
}

char* circleToString(void* data)
{
    Circle *temp = (Circle *)(data);
    char *temp1 = toString(temp->otherAttributes);

    int size = strlen("Circle\nCX:   CY:   R:   units: \n**OTHER ATTRIBUTES**\n") + strlen(temp->units)
    + strlen(temp1) + 37;
    char *string = malloc(size);
    snprintf(string, size - 1, "Circle\nCX: %f  CY: %f  R: %f  units: %s\n**OTHER ATTRIBUTES**%s\n"
    , temp->cx, temp->cy, temp->r, temp->units, temp1);
    free(temp1);

    return string;
}

int compareCircles(const void *first, const void *second)
{
    Circle *cmp1 = (Circle *)first;
    Circle *cmp2 = (Circle *)second;

    if(cmp1->cx != cmp2->cx)
        return 1;
    else if(cmp1->cy != cmp2->cy)
        return 1;
    else if(cmp1->r != cmp2->r)
        return 1;
    else
        return 0;
}

// PATH
void deletePath(void* data)
{
    Path *temp = (Path *)data;
    free(temp->data);
    freeList(temp->otherAttributes);
    free(data);
}

char* pathToString(void* data)
{
    Path *temp = (Path *)(data);
    char *temp1 = toString(temp->otherAttributes);
    int size = strlen("Path\nData: \n**OTHER ATTRIBUTES**\n\n") + strlen(temp->data)
    + strlen(temp1) + 1;
    char *string = malloc(size);
    snprintf(string, size - 1, "Path\nData: %s\n**OTHER ATTRIBUTES**%s\n\n"
    , temp->data, temp1);

    free(temp1);

    return string;
}

int comparePaths(const void *first, const void *second)
{
    Path *cmp1 = (Path *)first;
    Path *cmp2 = (Path *)second;

    if(strcmp(cmp1->data, cmp2->data) != 0)
        return 1;
    else
        return 0;
}

float stringToFloat(char * toChange)
{
    float num;
    int i = 0;
    char *end;

    while((toChange[i] < 65) && (toChange[i] != 0))
        i++;

    end = &(toChange[i]);
    
    num = strtof(toChange, &end);

    return num;
}

char* unitsFromString(char * string)
{
    char *units = malloc(50);
    while((*string < 65) && (*string != 0))
        string++;
    
    if(*string)
        strcpy(units, string);
    else
        strcpy(units, "");
    
    return units;
}

void freeTheList(List *list)
{
    Node *temp = list->head;
    while(temp)
    {
        Node *next = temp->next;
        free(temp);
        temp = next;
    }
    free(list);
}

char *getSVG(char *filename)
{
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");
    char *json = SVGtoJSON(image);
    deleteSVGimage(image);
    return json;
}

char *getTitle(char *filename)
{
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");

    if(image)
    {
        char *title = malloc(sizeof(image->title) + 1);
        strcpy(title, image->title);
        deleteSVGimage(image);
        return title;
    }
    return NULL;
}

char *getDesc(char *filename)
{
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");

    if(image)
    {
        char *desc = malloc(sizeof(image->description) + 1);
        strcpy(desc, image->description);
        deleteSVGimage(image);
        return desc;
    }
    return NULL;
}

char *getJSONRects(char *filename)
{
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");

    if(image)
    {
        List *rects = getRects(image);
        char *list = rectListToJSON(rects);
        deleteSVGimage(image);
        freeTheList(rects);
        return list;
    }
    return NULL;
}

char *getJSONCircles(char *filename)
{
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");

    if(image)
    {
        List *circles = getCircles(image);
        char *list = circListToJSON(circles);
        deleteSVGimage(image);
        freeTheList(circles);
        return list;
    }
    return NULL;
}

char *getJSONPaths(char *filename)
{
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");

    if(image)
    {
        List *paths = getPaths(image);
        char *list = pathListToJSON(paths);
        deleteSVGimage(image);
        freeTheList(paths);
        return list;
    }
    return NULL;
}

char *getJSONGroups(char *filename)
{
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");

    if(image)
    {
        List *groups = getGroups(image);
        char *list = groupListToJSON(groups);
        deleteSVGimage(image);
        freeTheList(groups);
        return list;
    }
    return NULL;
}

char *editAttribute(char *filename, char *component, char *attr, char *value)
{
    int index = component[strlen(component) - 1] - 49;
    char *theComponent = malloc(strlen(component) - 1);
    char *success = malloc(strlen("success") + 1);
    char *fail = malloc(strlen("fail") + 1);
    Attribute *toAdd = malloc(sizeof(Attribute));
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");

    strcpy(success ,"success");
    strcpy(fail, "fail");
    strncpy(theComponent, component, strlen(component) - 2);
    toAdd->name = malloc(strlen(attr) + 1);
    toAdd->value = malloc(strlen(value) + 1);
    strcpy(toAdd->name, attr);
    strcpy(toAdd->value, value);

    if(strcmp(theComponent, "Rectangle") == 0)
        setAttribute(image, RECT, index, toAdd);
    else if(strcmp(theComponent, "Circle") == 0)
        setAttribute(image, CIRC, index, toAdd);
    else if(strcmp(theComponent, "Path") == 0)
        setAttribute(image, PATH, index, toAdd);
    else if(strcmp(theComponent, "Group") == 0)
        setAttribute(image, GROUP, index, toAdd);
    else
    {
        free(theComponent);
        free(success);
        deleteSVGimage(image);
        return fail;
    }

    if(validateSVGimage(image, "schemas/svg.xsd") == false)
    {
        free(theComponent);
        free(success);
        deleteSVGimage(image);
        return fail;
    }

    if(remove(filename) != 0)
    {
        free(theComponent);
        free(success);
        deleteSVGimage(image);
        return fail;
    }

    if(writeSVGimage(image, filename) == false)
    {
        free(theComponent);
        free(success);
        deleteSVGimage(image);
        return fail;
    }
    free(fail);
    free(theComponent);
    deleteSVGimage(image);

    return success;
}

char *editTitle(char *filename, char *title)
{
    char *success = malloc(strlen("success") + 1);
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");
    strcpy(success ,"success");

    if(validateSVGimage(image, "schemas/svg.xsd") == false)
    {
        free(success);
        deleteSVGimage(image);
        return NULL;
    }

    memset(image->title, 0, 256);
    if(strlen(title) < 256)
    {
        strcpy(image->title, title);
        if(validateSVGimage(image, "schemas/svg.xsd") == false)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }

        if(remove(filename) != 0)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }

        if(writeSVGimage(image, filename) == false)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }
        deleteSVGimage(image);
        return success;
    }
    free(success);
    return NULL;
}

char *editDescription(char *filename, char *description)
{
    char *success = malloc(strlen("success") + 1);
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");
    strcpy(success ,"success");

    if(validateSVGimage(image, "schemas/svg.xsd") == false)
    {
        free(success);
        deleteSVGimage(image);
        return NULL;
    }

    memset(image->description, 0, 256);
    if(strlen(description) < 256)
    {
        strcpy(image->description, description);
        if(validateSVGimage(image, "schemas/svg.xsd") == false)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }

        if(remove(filename) != 0)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }

        if(writeSVGimage(image, filename) == false)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }
        deleteSVGimage(image);
        return success;
    }
    free(success);
    return NULL;
}

//Add a default shape to an image
char *addShape(char *filename, char *shape)
{
    char *success = malloc(strlen("success") + 1);
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");
    strcpy(success ,"success");

    if(validateSVGimage(image, "schemas/svg.xsd") == false)
    {
        free(success);
        deleteSVGimage(image);
        return NULL;
    }

    if(strcmp(shape, "Rectangle") == 0)
    {
        Rectangle *rect = malloc(sizeof(Rectangle));
        rect->x = 1.0;
        rect->y = 1.0;
        rect->width = 1.0;
        rect->height = 1.0;
        strcpy(rect->units, "cm");
        rect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
        insertBack(image->rectangles, (void *)rect);

        if(validateSVGimage(image, "schemas/svg.xsd") == false)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }

        if(remove(filename) != 0)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }

        if(writeSVGimage(image, filename) == false)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }
        deleteSVGimage(image);
        return success;
    }
    else if(strcmp(shape, "Circle") == 0)
    {
        Circle *circ = malloc(sizeof(Circle));
        circ->cx = 1.0;
        circ->cy = 1.0;
        circ->r = 1.0;
        strcpy(circ->units, "cm");
        circ->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
        insertBack(image->circles, (void *)circ);

        if(validateSVGimage(image, "schemas/svg.xsd") == false)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }

        if(remove(filename) != 0)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }

        if(writeSVGimage(image, filename) == false)
        {
            free(success);
            deleteSVGimage(image);
            return NULL;
        }
        deleteSVGimage(image);
        return success;
    }
    else
    {
        free(success);
        deleteSVGimage(image);
        return NULL;
    }
}

char *addSVG(char *filename)
{
    SVGimage *image = malloc(sizeof(SVGimage));
    char *success = malloc(strlen("success") + 1);
    strcpy(success ,"success");

    strcpy(image->title, "");
    strcpy(image->description, "");
    image->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    image->circles = initializeList(circleToString, deleteCircle, compareCircles);
    image->paths = initializeList(pathToString, deletePath, comparePaths);
    image->groups = initializeList(groupToString, deleteGroup, compareGroups);
    image->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    strcpy(image->namespace, "http://www.w3.org/2000/svg");

    if(validateSVGimage(image, "schemas/svg.xsd") == false)
    {
        deleteSVGimage(image);
        return NULL;
    }

    if(writeSVGimage(image, filename) == false)
    {
        deleteSVGimage(image);
        return NULL;
    }

    deleteSVGimage(image);
    return success;
}

char *scaleShape(char *filename, char *shape, char *size)
{
    int index = shape[strlen(shape) - 1] - 48;
    char *theComponent = malloc(strlen(shape) - 1);
    char *success = malloc(strlen("success") + 1);
    char *fail = malloc(strlen("fail") + 1);
    SVGimage *image = createValidSVGimage(filename, "schemas/svg.xsd");
    float scale = strtof(size, NULL);
    strcpy(success ,"success");
    strcpy(fail, "fail");
    strncpy(theComponent, shape, strlen(shape) - 2);

    //Check scale is between 0 and 10
    if((scale > 10) || (scale < 0))
    {
        free(theComponent);
        free(success);
        deleteSVGimage(image);
        return fail;
    }

    if(strcmp(theComponent, "Rectangle") == 0)
    {
        ListIterator list = createIterator(image->rectangles);
        Rectangle *rect;
        int i = 0;

        while(i < index)
        {
            rect = (Rectangle *)nextElement(&list);
            i++;
        }
        rect->width = rect->width * scale;
        rect->height = rect->height * scale;
    }
    else if(strcmp(theComponent, "Circle") == 0)
    {
        ListIterator list = createIterator(image->circles);
        Circle *circ;
        int i = 0;

        while(i < index)
        {
            circ = (Circle *)nextElement(&list);
            i++;
        }
        circ->r = circ->r * scale;
    }
    else
    {
        free(theComponent);
        free(success);
        deleteSVGimage(image);
        return fail;
    }

    if(validateSVGimage(image, "schemas/svg.xsd") == false)
    {
        free(theComponent);
        free(success);
        deleteSVGimage(image);
        return fail;
    }

    if(remove(filename) != 0)
    {
        free(theComponent);
        free(success);
        deleteSVGimage(image);
        return fail;
    }

    if(writeSVGimage(image, filename) == false)
    {
        free(theComponent);
        free(success);
        deleteSVGimage(image);
        return fail;
    }
    free(fail);
    free(theComponent);
    deleteSVGimage(image);

    return success;
}

