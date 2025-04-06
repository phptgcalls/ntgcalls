/*
 // Created by Laky64 on 12/08/2023.
*/

#include <phpcpp.h>
#include "../ntgcalls.hpp"
#include "ntgcalls/exceptions.hpp"
#include "../models/rtc_server.hpp"
#include <stdexcept>
#include <vector>

using namespace ntgcalls;


// Forward declarations for helper functions
void register_stream_type_constants(Php::Namespace &ntgcallsNamespace);
void register_stream_status_constants(Php::Namespace &ntgcallsNamespace);
void register_connection_state_constants(Php::Namespace &ntgcallsNamespace);
void register_input_mode_constants(Php::Namespace &ntgcallsNamespace);
void register_exceptions(Php::Namespace &ntgcallsNamespace);

// PHP wrapper for the NTgCalls C++ class
class PHPNTgCalls : public Php::Base {
private:
    NTgCalls *ntg;

public:
    PHPNTgCalls() {
        try {
            ntg = new NTgCalls();
        } catch(const std::exception &e) {
            // Optionally throw a PHP exception if instantiation fails
            throw Php::Exception(e.what());
        }
    }

    ~PHPNTgCalls() {
        delete ntg;
    }
    /*
    // Convert a single Php::Value (expected to be an object) into an RTCServer
    ntgcalls::RTCServer convertPhpObjectToRTCServer(const Php::Value &val) {
        if (!val.isObject()) {
            throw std::invalid_argument("Expected an object for RTCServer conversion");
        }
        Php::Object obj = val;
        // Adjust these keys to match your object's properties:
        int id = obj.get("id").numericValue();
        std::string ip = obj.get("ip").stringValue();
        int port = obj.get("port").numericValue();
        return ntgcalls::RTCServer(id, ip, port);
    }
    
    // Convert a PHP array into a vector of RTCServer objects
    std::vector<ntgcalls::RTCServer> convertPhpArrayToRTCServerVector(const Php::Value &val) {
        std::vector<ntgcalls::RTCServer> vec;
        if (!val.isArray()) {
            throw std::invalid_argument("Expected an array for RTCServer vector conversion");
        }
        Php::Array arr = val;
        for (auto iter = arr.begin(); iter != arr.end(); ++iter) {
            vec.push_back(convertPhpObjectToRTCServer(iter->second));
        }
        return vec;
    }
    Php::Value create_p2p_call(Php::Parameters &params) {
        try {
            // Note: You may need to convert PHP objects (dh_config, media) to their C++ counterparts.
            auto result = ntg->createP2PCall(
                params["user_id"].numericValue(),
                params["dh_config"],
                params["g_a_hash"].stringValue(),
                params["media"]
            );
            return result;
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value exchange_keys(Php::Parameters &params) {
        try {
            auto result = ntg->exchangeKeys(
                params["user_id"].numericValue(),
                params["g_a_or_b"].stringValue(),
                params["fingerprint"].stringValue()
            );
            return result;
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value connect_p2p(Php::Parameters &params) {
        try {
            int user_id = params["user_id"].numericValue();
            bool p2p_allowed = params["p2p_allowed"].boolValue();
            
            // Convert "servers" from PHP array to std::vector<RTCServer>
            std::vector<ntgcalls::RTCServer> servers = convertPhpArrayToRTCServerVector(params["servers"]);
    
            // Convert "versions" from PHP array to std::vector<int>
            std::vector<int> versions;
            if (params["versions"].isArray()) {
                Php::Array arr = params["versions"];
                for (auto iter = arr.begin(); iter != arr.end(); ++iter) {
                    versions.push_back(iter->second.numericValue());
                }
            }
            
            // Now call the C++ method with the converted parameters.
            auto result = ntg->connectP2P(user_id, servers, versions, p2p_allowed);
            return result;
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }    

    Php::Value send_signaling(Php::Parameters &params) {
        try {
            auto result = ntg->sendSignalingData(
                params["chat_id"].numericValue(),
                params["msg_key"].stringValue()
            );
            return result;
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value create_call(Php::Parameters &params) {
        try {
            auto result = ntg->createCall(
                params["chat_id"].numericValue(),
                params["media"]
            );
            return result;
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value connect(Php::Parameters &params) {
        try {
            auto result = ntg->connect(
                params["chat_id"].numericValue(),
                params["params"]
            );
            return result;
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value change_stream(Php::Parameters &params) {
        try {
            auto result = ntg->changeStream(
                params["chat_id"].numericValue(),
                params["media"]
            );
            return result;
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value pause(Php::Parameters &params) {
        try {
            return ntg->pause(params["chat_id"].numericValue());
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value resume(Php::Parameters &params) {
        try {
            return ntg->resume(params["chat_id"].numericValue());
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value mute(Php::Parameters &params) {
        try {
            return ntg->mute(params["chat_id"].numericValue());
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value unmute(Php::Parameters &params) {
        try {
            return ntg->unmute(params["chat_id"].numericValue());
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value stop(Php::Parameters &params) {
        try {
            return ntg->stop(params["chat_id"].numericValue());
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value time(Php::Parameters &params) {
        try {
            return ntg->time(params["chat_id"].numericValue());
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value get_state(Php::Parameters &params) {
        try {
            return ntg->getState(params["chat_id"].numericValue());
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    // Placeholder: These functions need proper callback binding logic.
    Php::Value on_upgrade(Php::Parameters &params) {
        // Bind and store the callback for upgrade events.
        return Php::Value();
    }

    Php::Value on_stream_end(Php::Parameters &params) {
        // Bind and store the callback for stream end events.
        return Php::Value();
    }

    Php::Value on_connection_change(Php::Parameters &params) {
        // Bind and store the callback for connection change events.
        return Php::Value();
    }

    Php::Value on_signaling(Php::Parameters &params) {
        // Bind and store the callback for signaling events.
        return Php::Value();
    }

    Php::Value calls() {
        try {
            return ntg->calls();
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    Php::Value cpu_usage() {
        try {
            return ntg->cpuUsage();
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }

    static Php::Value ping() {
        try {
            return NTgCalls::ping();
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }
    */
    static Php::Value get_protocol() {
        try {
            // Retrieve the protocol (assumed to be of type ntgcalls::Protocol)
            auto protocol = NTgCalls::getProtocol();
            Php::Array arr;
            arr["maxLayer"] = protocol.max_layer;
            return arr;
        } catch (const std::exception &e) {
            throw Php::Exception(e.what());
        }
    }
};

// Module Initialization using PHP-CPP
extern "C" {
    PHPCPP_EXPORT void *get_module() {
        static Php::Extension extension("ntgcalls", "1.0");

        // Define NTgCalls class and register its methods
        Php::Class<PHPNTgCalls> ntgCallsClass("NTgCalls");
        /*
        ntgCallsClass.method<&PHPNTgCalls::create_p2p_call>("create_p2p_call", {
            Php::ByVal("user_id", Php::Type::Numeric, true),
            Php::ByVal("dh_config", Php::Type::Object, true),
            Php::ByVal("g_a_hash", Php::Type::String, true),
            Php::ByVal("media", Php::Type::Object, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::exchange_keys>("exchange_keys", {
            Php::ByVal("user_id", Php::Type::Numeric, true),
            Php::ByVal("g_a_or_b", Php::Type::String, true),
            Php::ByVal("fingerprint", Php::Type::String, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::connect_p2p>("connect_p2p", {
            Php::ByVal("user_id", Php::Type::Numeric, true),
            Php::ByVal("servers", Php::Type::Array, true),
            Php::ByVal("versions", Php::Type::Array, true),
            Php::ByVal("p2p_allowed", Php::Type::Bool, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::send_signaling>("send_signaling", {
            Php::ByVal("chat_id", Php::Type::Numeric, true),
            Php::ByVal("msg_key", Php::Type::String, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::create_call>("create_call", {
            Php::ByVal("chat_id", Php::Type::Numeric, true),
            Php::ByVal("media", Php::Type::Object, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::connect>("connect", {
            Php::ByVal("chat_id", Php::Type::Numeric, true),
            Php::ByVal("params", Php::Type::Array, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::change_stream>("change_stream", {
            Php::ByVal("chat_id", Php::Type::Numeric, true),
            Php::ByVal("media", Php::Type::Object, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::pause>("pause", {
            Php::ByVal("chat_id", Php::Type::Numeric, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::resume>("resume", {
            Php::ByVal("chat_id", Php::Type::Numeric, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::mute>("mute", {
            Php::ByVal("chat_id", Php::Type::Numeric, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::unmute>("unmute", {
            Php::ByVal("chat_id", Php::Type::Numeric, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::stop>("stop", {
            Php::ByVal("chat_id", Php::Type::Numeric, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::time>("time", {
            Php::ByVal("chat_id", Php::Type::Numeric, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::get_state>("get_state", {
            Php::ByVal("chat_id", Php::Type::Numeric, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::on_upgrade>("on_upgrade", {
            Php::ByVal("callback", Php::Type::Callable, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::on_stream_end>("on_stream_end", {
            Php::ByVal("callback", Php::Type::Callable, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::on_connection_change>("on_connection_change", {
            Php::ByVal("callback", Php::Type::Callable, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::on_signaling>("on_signaling", {
            Php::ByVal("callback", Php::Type::Callable, true)
        });
        ntgCallsClass.method<&PHPNTgCalls::calls>("calls");
        ntgCallsClass.method<&PHPNTgCalls::cpu_usage>("cpu_usage");
        ntgCallsClass.method<&PHPNTgCalls::ping>("ping", Php::Public | Php::Static);
        */
        ntgCallsClass.method<&PHPNTgCalls::get_protocol>("get_protocol", Php::Public | Php::Static);

        extension.add(std::move(ntgCallsClass));

        // Define constants and exceptions in the ntgcalls namespace.
        Php::Namespace ntgcallsNamespace("ntgcalls");
        register_stream_type_constants(ntgcallsNamespace);
        register_stream_status_constants(ntgcallsNamespace);
        register_connection_state_constants(ntgcallsNamespace);
        register_input_mode_constants(ntgcallsNamespace);
        register_exceptions(ntgcallsNamespace);
        ntgcallsNamespace.add(Php::Constant("VERSION_INFO", "1.0"));
        extension.add(std::move(ntgcallsNamespace));

        return extension.module();
    }
}

// Helper functions to register enumerations as constants
void register_stream_type_constants(Php::Namespace &ntgcallsNamespace) {
    ntgcallsNamespace.add(Php::Constant("StreamType_AUDIO", static_cast<int>(Stream::Type::Audio)));
    ntgcallsNamespace.add(Php::Constant("StreamType_VIDEO", static_cast<int>(Stream::Type::Video)));
}

void register_stream_status_constants(Php::Namespace &ntgcallsNamespace) {
    ntgcallsNamespace.add(Php::Constant("StreamStatus_PLAYING", static_cast<int>(Stream::Status::Playing)));
    ntgcallsNamespace.add(Php::Constant("StreamStatus_PAUSED", static_cast<int>(Stream::Status::Paused)));
    ntgcallsNamespace.add(Php::Constant("StreamStatus_IDLING", static_cast<int>(Stream::Status::Idling)));
}

void register_connection_state_constants(Php::Namespace &ntgcallsNamespace) {
    ntgcallsNamespace.add(Php::Constant("ConnectionState_CONNECTING", static_cast<int>(CallInterface::ConnectionState::Connecting)));
    ntgcallsNamespace.add(Php::Constant("ConnectionState_CONNECTED", static_cast<int>(CallInterface::ConnectionState::Connected)));
    ntgcallsNamespace.add(Php::Constant("ConnectionState_FAILED", static_cast<int>(CallInterface::ConnectionState::Failed)));
    ntgcallsNamespace.add(Php::Constant("ConnectionState_TIMEOUT", static_cast<int>(CallInterface::ConnectionState::Timeout)));
    ntgcallsNamespace.add(Php::Constant("ConnectionState_CLOSED", static_cast<int>(CallInterface::ConnectionState::Closed)));
}

void register_input_mode_constants(Php::Namespace &ntgcallsNamespace) {
    ntgcallsNamespace.add(Php::Constant("InputMode_FILE", static_cast<int>(BaseMediaDescription::InputMode::File)));
    ntgcallsNamespace.add(Php::Constant("InputMode_SHELL", static_cast<int>(BaseMediaDescription::InputMode::Shell)));
    ntgcallsNamespace.add(Php::Constant("InputMode_FFMPEG", static_cast<int>(BaseMediaDescription::InputMode::FFmpeg)));
    ntgcallsNamespace.add(Php::Constant("InputMode_NO_LATENCY", static_cast<int>(BaseMediaDescription::InputMode::NoLatency)));
}

// Helper function to register exception classes
void register_exceptions(Php::Namespace &ntgcallsNamespace) {
    // Register base exception
}
