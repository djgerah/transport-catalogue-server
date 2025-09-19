#include "../include/server_handlers.h"
#include "../include/json_reader.h"

void HandleLoad(const std::string &body, ServerState &state)
{
    std::istringstream input(body);
    state.catalogue = std::make_unique<tc::TransportCatalogue>();
    state.json_reader = std::make_unique<json_reader::JsonReader>(input);
    state.json_reader->FillTransportCatalogue(*state.catalogue);

    renderer::RenderSettings settings = state.json_reader->FillRenderSettings(state.json_reader->GetRenderSettings());
    state.renderer = std::make_unique<renderer::MapRenderer>(settings);

    auto routing_settings = state.json_reader->FillRoutingSettings(state.json_reader->GetRoutingSettings());
    state.router = std::make_unique<tc::TransportRouter>(routing_settings, *state.catalogue);

    state.request_handler = std::make_unique<RequestHandler>(*state.catalogue, *state.renderer, *state.router);
}

void HandleQuery(const std::string &body, httplib::Response &res, ServerState &state)
{
    if (!state.request_handler)
    {
        res.status = 400;
        res.set_content("{\"error\":\"catalogue not loaded\"}", "application/json");
        return;
    }

    try
    {
        std::istringstream input(body);
        std::ostringstream output;

        json_reader::JsonReader document(input);
        document.ProcessRequests(document.GetStatRequests(), *state.catalogue, *state.request_handler, output);

        res.set_content(output.str(), "application/json");
    }
    catch (const std::exception &e)
    {
        res.status = 500;
        res.set_content(std::string("{\"error\":\"") + e.what() + "\"}", "application/json");
    }
}

void HandleMap(httplib::Response &res, ServerState &state)
{
    if (!state.renderer || !state.catalogue)
    {
        res.status = 400;
        res.set_content("{\"error\":\"catalogue not loaded\"}", "application/json");
        return;
    }

    try
    {
        auto svg_doc = state.renderer->GetSVG(state.catalogue->GetAllBuses());
        std::ostringstream out;
        svg_doc.Render(out);
        res.set_content(out.str(), "image/svg+xml");
    }

    catch (const std::exception &e)
    {
        res.status = 500;
        res.set_content(std::string("{\"error\":\"") + e.what() + "\"}", "application/json");
    }
}

void HandlePutStop(const std::string &body, ServerState &state)
{
    if (!state.catalogue || !state.json_reader)
        return;

    std::istringstream input(body);
    json::Document doc = json::Load(input);
    const auto &base_requests = doc.GetRoot().AsDict().at("base_requests").AsArray();

    for (const auto &request : base_requests)
    {
        state.json_reader->ParseRequest(request);
    }

    state.json_reader->ApplyCommands(*state.catalogue);
}

void HandlePutBus(const std::string &body, ServerState &state)
{
    if (!state.catalogue || !state.json_reader)
        return;

    std::istringstream input(body);
    json::Document doc = json::Load(input);
    const auto &base_requests = doc.GetRoot().AsDict().at("base_requests").AsArray();

    for (const auto &request : base_requests)
    {
        state.json_reader->ParseRequest(request);
    }

    state.json_reader->ApplyCommands(*state.catalogue);
}