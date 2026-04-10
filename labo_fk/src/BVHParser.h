#pragma once

#include "Armature.h"
#include "Math3D.h"

namespace gti320
{
    // Simple BVH parser.
    // 
    // Adapted from the BVH View source by Daniel Holden 
    // https://github.com/orangeduck/BVHView
    //

    enum
    {
        PARSER_ERR_MAX = 512,
    };

    // Parser struct
    typedef struct {

        const char* filename;
        int offset;
        const char* data;
        int row;
        int col;
        char err[PARSER_ERR_MAX];

    } Parser;

    // Initialize the Parser
    static inline void ParserInit(Parser* par, const char* filename, const char* data)
    {
        par->filename = filename;
        par->offset = 0;
        par->data = data;
        par->row = 0;
        par->col = 0;
        par->err[0] = '\0';
    }

    // Peek at the next character in the stream
    static inline char ParserPeek(const Parser* par)
    {
        return par->data[par->offset];
    }

    // Peek forward N steps in the stream. Does not check the stream is long enough.
    static inline char ParserPeekForward(const Parser* par, int steps)
    {
        return par->data[par->offset + steps];
    }

    // Checks the current character matches the given input
    static inline bool ParserMatch(const Parser* par, char match)
    {
        return match == par->data[par->offset];
    }

    // Checks the current character matches one of the given characters
    static inline bool ParserOneOf(const Parser* par, const char* matches)
    {
        return strchr(matches, par->data[par->offset]);
    }

    // Checks the following characters in the stream match the prefix (in a caseless way)
    static inline bool ParserStartsWithCaseless(const Parser* par, const char* prefix)
    {
        const char* start = par->data + par->offset;
        while (*prefix)
        {
            if (tolower(*prefix) != tolower(*start)) { return false; }
            prefix++;
            start++;
        }

        return true;
    }

    // Advances the stream forward one
    static inline void ParserInc(Parser* par)
    {
        if (par->data[par->offset] == '\n')
        {
            par->row++;
            par->col = 0;
        }
        else
        {
            par->col++;
        }

        par->offset++;
    }

    // Advances the stream forward "num" characters
    static inline void ParserAdvance(Parser* par, int num)
    {
        for (int i = 0; i < num; i++) { ParserInc(par); }
    }

    // Gets the human readable name of a particular character
    static inline char* ParserCharName(char c)
    {
        static char parserCharName[2];

        switch (c)
        {
        case '\0': return "end of file";
        case '\r': return "new line";
        case '\n': return "new line";
        case '\t': return "tab";
        case '\v': return "vertical tab";
        case '\b': return "backspace";
        case '\f': return "form feed";
        default:
            parserCharName[0] = c;
            parserCharName[1] = '\0';
            return parserCharName;
        }
    }

    // Prints a formatted error to the parser error buffer
#define ParserError(par, fmt, ...) \
    snprintf(par->err, PARSER_ERR_MAX, "%s:%i:%i: error: " fmt, par->filename, par->row, par->col, ##__VA_ARGS__)

//----------------------------------------------------------------------------------
// BVH File Data
//----------------------------------------------------------------------------------

// Types of "channels" that are possible in the BVH format
    enum
    {
        CHANNEL_X_POSITION = 0,
        CHANNEL_Y_POSITION = 1,
        CHANNEL_Z_POSITION = 2,
        CHANNEL_X_ROTATION = 3,
        CHANNEL_Y_ROTATION = 4,
        CHANNEL_Z_ROTATION = 5,
        CHANNELS_MAX = 6,
    };

    // Data associated with a single "joint" in the BVH format
    typedef struct
    {
        int parent;
        char name[64];
        Vector3f offset;
        int channelCount;
        char channels[CHANNELS_MAX];
        bool endSite;

    } BVHJointData;

    static inline void BVHJointDataInit(BVHJointData* data)
    {
        data->parent = -1;
        data->offset.setZero();
        data->channelCount = 0;
        data->endSite = false;
    }

    static inline void BVHJointDataRename(BVHJointData* data, const char* name)
    {
        strncpy(data->name, name, 64);
    }

    static inline void BVHJointDataFree(BVHJointData* data)
    {
    }

    // Data structure matching what is present in the BVH file format
    typedef struct
    {
        // Hierarchy Data

        int jointCount;
        BVHJointData* joints;

        // Motion Data

        int frameCount;
        int channelCount;
        float frameTime;
        float* motionData;

    } BVHData;

    static inline void BVHDataInit(BVHData* bvh)
    {
        bvh->jointCount = 0;
        bvh->joints = NULL;
        bvh->frameCount = 0;
        bvh->channelCount = 0;
        bvh->frameTime = 0.0f;
        bvh->motionData = NULL;
    }

    static inline void BVHDataFree(BVHData* bvh)
    {
        for (int i = 0; i < bvh->jointCount; i++)
        {
            BVHJointDataFree(&bvh->joints[i]);
        }
        
        delete[] bvh->joints;

        delete[] bvh->motionData;
    }

    static inline int BVHDataAddJoint(BVHData* bvh)
    {
        bvh->joints = (BVHJointData*)realloc(bvh->joints, (bvh->jointCount + 1) * sizeof(BVHJointData));
        bvh->jointCount++;
        BVHJointDataInit(&bvh->joints[bvh->jointCount - 1]);
        return bvh->jointCount - 1;
    }

    //----------------------------------------------------------------------------------
    // BVH Parser
    //----------------------------------------------------------------------------------

    // Parse any whitespace
    static void BVHParseWhitespace(Parser* par)
    {
        while (ParserOneOf(par, " \r\t\v")) { ParserInc(par); }
    }

    // Parse the given string (in a non-case sensitive way). I've found that in practice
    // many BVH files don't respect case sensitivity so parsing any keywords in a non-case
    // sensitive way seems safer.
    static bool BVHParseString(Parser* par, const char* string)
    {
        if (ParserStartsWithCaseless(par, string))
        {
            ParserAdvance(par, strlen(string));
            return true;
        }
        else
        {
            ParserError(par, "expected '%s' at '%s'", string, ParserCharName(ParserPeek(par)));
            return false;
        }
    }

    // Parse any whitespace followed by a newline
    static bool BVHParseNewline(Parser* par)
    {
        BVHParseWhitespace(par);

        if (ParserMatch(par, '\n'))
        {
            ParserInc(par);
            BVHParseWhitespace(par);
            return true;
        }
        else
        {
            ParserError(par, "expected newline at '%s'", ParserCharName(ParserPeek(par)));
            return false;
        }
    }

    // Parse any whitespace and then an identifier for the name of a joint
    static bool BVHParseJointName(BVHJointData* jnt, Parser* par)
    {
        BVHParseWhitespace(par);

        char buffer[256];
        int chrnum = 0;
        while (chrnum < 255 && ParserOneOf(par,
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_:-."))
        {
            buffer[chrnum] = ParserPeek(par);
            chrnum++;
            ParserInc(par);
        }
        buffer[chrnum] = '\0';

        if (chrnum > 0)
        {
            BVHJointDataRename(jnt, buffer);
            BVHParseWhitespace(par);
            return true;
        }
        else
        {
            ParserError(par, "expected joint name at '%s'", ParserCharName(ParserPeek(par)));
            return false;
        }
    }

    // Parse a float value
    static bool BVHParseFloat(float* out, Parser* par)
    {
        BVHParseWhitespace(par);

        char* end;
        errno = 0;
        (*out) = strtod(par->data + par->offset, &end);

        if (errno == 0)
        {
            ParserAdvance(par, end - (par->data + par->offset));
            return true;
        }
        else
        {
            ParserError(par, "expected float at '%s'", ParserCharName(ParserPeek(par)));
            return false;
        }
    }

    // Parse an integer value
    static bool BVHParseInt(int* out, Parser* par)
    {
        BVHParseWhitespace(par);

        char* end;
        errno = 0;
        (*out) = (int)strtol(par->data + par->offset, &end, 10);

        if (errno == 0)
        {
            ParserAdvance(par, end - (par->data + par->offset));
            return true;
        }
        else
        {
            ParserError(par, "expected integer at '%s'", ParserCharName(ParserPeek(par)));
            return false;
        }
    }

    // Parse the "joint offset" part of the BVH File
    static bool BVHParseJointOffset(BVHJointData* jnt, Parser* par)
    {
        if (!BVHParseString(par, "OFFSET")) { return false; }
        if (!BVHParseFloat(&jnt->offset(0), par)) { return false; }
        if (!BVHParseFloat(&jnt->offset(1), par)) { return false; }
        if (!BVHParseFloat(&jnt->offset(2), par)) { return false; }
        if (!BVHParseNewline(par)) { return false; }
        return true;
    }

    // Parse a channel type and return it in "channel"
    static bool BVHParseChannelEnum(
        char* channel,
        Parser* par,
        const char* channelName,
        char channelValue)
    {
        BVHParseWhitespace(par);
        if (!BVHParseString(par, channelName)) { return false; }
        BVHParseWhitespace(par);
        *channel = channelValue;
        return true;
    }

    // Parse a channel type and return it in "channel"
    static bool BVHParseChannel(char* channel, Parser* par)
    {
        BVHParseWhitespace(par);

        if (ParserPeek(par) == '\0')
        {
            ParserError(par, "expected channel at end of file");
            return false;
        }

        // Here we are safe to peek forward an extra character since we've already
        // checked the current character is not the null terminator.

        if (ParserPeek(par) == 'X' && ParserPeekForward(par, 1) == 'p')
        {
            return BVHParseChannelEnum(channel, par, "Xposition", CHANNEL_X_POSITION);
        }

        if (ParserPeek(par) == 'Y' && ParserPeekForward(par, 1) == 'p')
        {
            return BVHParseChannelEnum(channel, par, "Yposition", CHANNEL_Y_POSITION);
        }

        if (ParserPeek(par) == 'Z' && ParserPeekForward(par, 1) == 'p')
        {
            return BVHParseChannelEnum(channel, par, "Zposition", CHANNEL_Z_POSITION);
        }

        if (ParserPeek(par) == 'X' && ParserPeekForward(par, 1) == 'r')
        {
            return BVHParseChannelEnum(channel, par, "Xrotation", CHANNEL_X_ROTATION);
        }

        if (ParserPeek(par) == 'Y' && ParserPeekForward(par, 1) == 'r')
        {
            return BVHParseChannelEnum(channel, par, "Yrotation", CHANNEL_Y_ROTATION);
        }

        if (ParserPeek(par) == 'Z' && ParserPeekForward(par, 1) == 'r')
        {
            return BVHParseChannelEnum(channel, par, "Zrotation", CHANNEL_Z_ROTATION);
        }

        ParserError(par, "expected channel type");
        return false;
    }

    // Parse the "channels" part of the BVH file format
    static bool BVHParseJointChannels(BVHJointData* jnt, Parser* par)
    {
        if (!BVHParseString(par, "CHANNELS")) { return false; }
        if (!BVHParseInt(&jnt->channelCount, par)) { return false; }

        for (int i = 0; i < jnt->channelCount; i++)
        {
            if (!BVHParseChannel(&jnt->channels[i], par)) { return false; }
        }

        if (!BVHParseNewline(par)) { return false; }

        return true;
    }

    // Parse a joint in the BVH file format
    static bool BVHParseJoints(BVHData* bvh, int parent, Parser* par)
    {
        while (ParserOneOf(par, "JEje")) // Either "JOINT" or "End Site"
        {
            int j = BVHDataAddJoint(bvh);
            bvh->joints[j].parent = parent;

            if (ParserMatch(par, 'J'))
            {
                if (!BVHParseString(par, "JOINT")) { return false; }
                if (!BVHParseJointName(&bvh->joints[j], par)) { return false; }
                if (!BVHParseNewline(par)) { return false; }
                if (!BVHParseString(par, "{")) { return false; }
                if (!BVHParseNewline(par)) { return false; }
                if (!BVHParseJointOffset(&bvh->joints[j], par)) { return false; }
                if (!BVHParseJointChannels(&bvh->joints[j], par)) { return false; }
                if (!BVHParseJoints(bvh, j, par)) { return false; }
                if (!BVHParseString(par, "}")) { return false; }
                if (!BVHParseNewline(par)) { return false; }
            }
            else if (ParserMatch(par, 'E'))
            {
                bvh->joints[j].endSite = true;

                if (!BVHParseString(par, "End Site")) { return false; }
                BVHJointDataRename(&bvh->joints[j], "End Site");
                if (!BVHParseNewline(par)) { return false; }
                if (!BVHParseString(par, "{")) { return false; }
                if (!BVHParseNewline(par)) { return false; }
                if (!BVHParseJointOffset(&bvh->joints[j], par)) { return false; }
                if (!BVHParseString(par, "}")) { return false; }
                if (!BVHParseNewline(par)) { return false; }
            }
        }

        return true;
    }

    // Parse the frame count
    static bool BVHParseFrames(BVHData* bvh, Parser* par)
    {
        if (!BVHParseString(par, "Frames:")) { return false; }
        if (!BVHParseInt(&bvh->frameCount, par)) { return false; }
        if (!BVHParseNewline(par)) { return false; }
        return true;
    }

    // Parse the frame time
    static bool BVHParseFrameTime(BVHData* bvh, Parser* par)
    {
        if (!BVHParseString(par, "Frame Time:")) { return false; }
        if (!BVHParseFloat(&bvh->frameTime, par)) { return false; }
        if (!BVHParseNewline(par)) { return false; }
        if (bvh->frameTime == 0.0f) { bvh->frameTime = 1.0f / 60.0f; }
        return true;
    }

    // Parse the motion data part of the BVH file format
    static bool BVHParseMotionData(BVHData* bvh, Parser* par)
    {
        int channelCount = 0;
        for (int i = 0; i < bvh->jointCount; i++)
        {
            channelCount += bvh->joints[i].channelCount;
        }

        bvh->channelCount = channelCount;
        bvh->motionData = new float[bvh->frameCount * channelCount];

        for (int i = 0; i < bvh->frameCount; i++)
        {
            for (int j = 0; j < channelCount; j++)
            {
                if (!BVHParseFloat(&bvh->motionData[i * channelCount + j], par)) { return false; }
            }

            if (!BVHParseNewline(par)) { return false; }
        }

        return true;
    }

    // Parse the entire BVH file format
    static bool BVHParse(BVHData* bvh, Parser* par)
    {
        // Hierarchy Data

        if (!BVHParseString(par, "HIERARCHY")) { return false; }
        if (!BVHParseNewline(par)) { return false; }

        int j = BVHDataAddJoint(bvh);

        if (!BVHParseString(par, "ROOT")) { return false; }
        if (!BVHParseJointName(&bvh->joints[j], par)) { return false; }
        if (!BVHParseNewline(par)) { return false; }
        if (!BVHParseString(par, "{")) { return false; }
        if (!BVHParseNewline(par)) { return false; }
        if (!BVHParseJointOffset(&bvh->joints[j], par)) { return false; }
        if (!BVHParseJointChannels(&bvh->joints[j], par)) { return false; }
        if (!BVHParseJoints(bvh, j, par)) { return false; }
        if (!BVHParseString(par, "}")) { return false; }
        if (!BVHParseNewline(par)) { return false; }

        // Motion Data

        if (!BVHParseString(par, "MOTION")) { return false; }
        if (!BVHParseNewline(par)) { return false; }
        if (!BVHParseFrames(bvh, par)) { return false; }
        if (!BVHParseFrameTime(bvh, par)) { return false; }
        if (!BVHParseMotionData(bvh, par)) { return false; }

        return true;
    }

    // Load the given file and parse the contents as a BVH file.
    static bool BVHDataLoad(BVHData* bvh, const char* filename, char* errMsg, int errMsgSize)
    {
        // Read file Contents

        FILE* f = fopen(filename, "rb");

        if (f == NULL)
        {
            snprintf(errMsg, errMsgSize, "Error: Could not find file '%s'\n", filename);
            return false;
        }

        fseek(f, 0, SEEK_END);
        long int length = ftell(f);
        fseek(f, 0, SEEK_SET);
        char* buffer = new char[length + 1];
        fread(buffer, 1, length, f);
        buffer[length] = '\n';
        fclose(f);

        // Free and re-init in case we are re-using an old buffer 
        BVHDataFree(bvh);
        BVHDataInit(bvh);

        // Parse BVH
        Parser par;
        ParserInit(&par, filename, buffer);
        bool result = BVHParse(bvh, &par);

        // Free contents and return result
        delete[] buffer;

        if (!result)
        {
            snprintf(errMsg, errMsgSize, "Error: Could not parse BVH file:\n    %s", par.err);
        }
        else
        {
            errMsg[0] = '\0';
            printf("INFO: parsed '%s' successfully\n", filename);
        }

        return result;
    }

    // Convert from BVH data -> our Armature class
    static void BVHtoArmature(const BVHData* bvh, Armature& armature)
    {
        Vector3f zero;
        zero.setZero();

        armature.links.clear();
        armature.root = nullptr;

        std::vector<Link*> links(bvh->jointCount, nullptr);

        for (int i = 0; i < bvh->jointCount; ++i)
        {
            const BVHJointData& joint = bvh->joints[i];
            
            if (joint.parent == -1) // root
            {
                links[i] = new Link(joint.name, nullptr, zero, zero);
                armature.root = links[i];
            }
            else
            {
                assert(joint.parent < i);
                links[i] = new Link(joint.name, links[joint.parent], zero, 0.1f*joint.offset);
                links[i]->endEffector = joint.endSite;
            }
        }

        for (Link* l : links) 
        {
            armature.links.push_back(l);
        }
    }

}