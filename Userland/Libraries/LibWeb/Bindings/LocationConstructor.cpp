/*
 * Copyright (c) 2022, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibJS/Runtime/GlobalObject.h>
#include <LibWeb/Bindings/LocationConstructor.h>
#include <LibWeb/Bindings/LocationPrototype.h>
#include <LibWeb/HTML/Window.h>

namespace Web::Bindings {

LocationConstructor::LocationConstructor(JS::Realm& realm)
    : NativeFunction(*realm.intrinsics().function_prototype())
{
}

LocationConstructor::~LocationConstructor() = default;

JS::ThrowCompletionOr<JS::Value> LocationConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "Location");
}

JS::ThrowCompletionOr<JS::Object*> LocationConstructor::construct(FunctionObject&)
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::NotAConstructor, "Location");
}

void LocationConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    auto& window = verify_cast<HTML::Window>(realm.global_object());

    NativeFunction::initialize(realm);
    define_direct_property(vm.names.prototype, &window.cached_web_prototype("Location"), 0);
    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
}

}
