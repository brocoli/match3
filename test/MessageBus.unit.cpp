#include "catch.hpp"

#include "../src/MessageBus.h"


SCENARIO("listeners can be attached, triggered and detached", "[MessageBus]") {
    GIVEN("a clean messageBus with no listeners attached at start") {
        Match3::MessageBus messageBus;
        int listenerTriggerCount = 0;

        std::shared_ptr<Match3::MessageBus::Callback> cb = std::make_shared<Match3::MessageBus::Callback>(
            [&listenerTriggerCount](Match3::MessageBus::Key, Match3::MessageBus::Data) {
                ++listenerTriggerCount;
            }
        );

        AND_GIVEN("no listener is attached") {
            WHEN("we notify the root event") {
                messageBus.Notify(Match3::MessageBus::Key::_);

                THEN("the listener is not called") {
                    REQUIRE(listenerTriggerCount == 0);
                }
            }

            WHEN("we notify the leaf event") {
                messageBus.Notify(Match3::MessageBus::Key::_Test);

                THEN("the listener is not called") {
                    REQUIRE(listenerTriggerCount == 0);
                }
            }
        }

        AND_GIVEN("a listener is attached to a leaf event") {
            messageBus.Attach(Match3::MessageBus::Key::_Test, cb);

            WHEN("we notify the root event") {
                messageBus.Notify(Match3::MessageBus::Key::_);

                THEN("the listener is not called") {
                    REQUIRE(listenerTriggerCount == 0);
                }
            }

            WHEN("we notify the leaf event") {
                messageBus.Notify(Match3::MessageBus::Key::_Test);

                THEN("the listener is called") {
                    REQUIRE(listenerTriggerCount == 1);
                }
            }
        }

        AND_GIVEN("a listener is attached to the root event") {
            messageBus.Attach(Match3::MessageBus::Key::_, cb);

            WHEN("we notify the root event") {
                messageBus.Notify(Match3::MessageBus::Key::_);

                THEN("the listener is called") {
                    REQUIRE(listenerTriggerCount == 1);
                }
            }

            WHEN("we notify the leaf event") {
                messageBus.Notify(Match3::MessageBus::Key::_Test);

                THEN("the listener is called") {
                    REQUIRE(listenerTriggerCount == 1);
                }
            }
        }

        AND_GIVEN("a listener is attached to both the root event and a leaf event") {
            messageBus.Attach(Match3::MessageBus::Key::_, cb);
            messageBus.Attach(Match3::MessageBus::Key::_Test, cb);

            WHEN("we notify the root event") {
                messageBus.Notify(Match3::MessageBus::Key::_);

                THEN("the listener is called once") {
                    REQUIRE(listenerTriggerCount == 1);
                }
            }

            WHEN("we notify the leaf event") {
                messageBus.Notify(Match3::MessageBus::Key::_Test);

                THEN("The listener is called twice") {
                    REQUIRE(listenerTriggerCount == 2);
                }
            }
        }

        AND_GIVEN("a listener is attached and then detached") {
            messageBus.Attach(Match3::MessageBus::Key::_Test, cb);
            messageBus.Detach(Match3::MessageBus::Key::_Test, cb);

            WHEN("we notify the root event") {
                messageBus.Notify(Match3::MessageBus::Key::_);

                THEN("the listener is not called") {
                    REQUIRE(listenerTriggerCount == 0);
                }
            }

            WHEN("we notify the leaf event") {
                messageBus.Notify(Match3::MessageBus::Key::_Test);

                THEN("the listener is not called") {
                    REQUIRE(listenerTriggerCount == 0);
                }
            }
        }

        AND_GIVEN("we attach a listener to it, and then leak the listener") {
            {
                std::shared_ptr<Match3::MessageBus::Callback> cb = std::make_shared<Match3::MessageBus::Callback>(
                    [&listenerTriggerCount](Match3::MessageBus::Key, Match3::MessageBus::Data) {
                        ++listenerTriggerCount;
                    }
                );
                messageBus.Attach(Match3::MessageBus::Key::_Test, cb);
            }

            WHEN("we notify the event") {
                messageBus.Notify(Match3::MessageBus::Key::_Test);

                THEN("the listener is not called") {
                    REQUIRE(listenerTriggerCount == 0);
                }

                THEN("a leak is detected") {
                    REQUIRE(messageBus.GetLeakDetected() == true);
                }
            }
        }
    }
}
