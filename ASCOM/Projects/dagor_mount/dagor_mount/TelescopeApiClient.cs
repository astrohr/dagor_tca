using System;

using ASCOM.DagorApiClient;
using Newtonsoft.Json;
using ASCOM.Utilities;
using System.Text;
using System.Net.Http;


namespace ASCOM.DagorTelescope
{

    public class StateRepr : IRepr
    {

        // Definitions

        public class Celest
        {
            [JsonProperty("ra")]
            public double ra { get; set; }

            [JsonProperty("de")]
            public double de { get; set; }
        }

        public class Altaz
        {
            [JsonProperty("alt")]
            public double alt { get; set; }

            [JsonProperty("az")]
            public double az { get; set; }
        }

        // Data structure

        [JsonProperty("ready")]
        public bool ready { get; set; }

        [JsonProperty("current")]
        public Current current { get; set; }
        public class Current
        {

            [JsonProperty("chirality")]
            public string chirality { get; set; }

            [JsonProperty("slewing")]
            public bool slewing { get; set; }

            [JsonProperty("on_target")]
            public bool on_target { get; set; }

            [JsonProperty("at_home")]
            public bool at_home { get; set; }

            [JsonProperty("celest")]
            public Celest celest { get; set; }
           
            [JsonProperty("altaz")]
            public Altaz altaz { get; set; }
        }

        [JsonProperty("config")]
        public Config config { get; set; }
        public class Config
        {
            [JsonProperty("tracking")]
            public bool tracking { get; set; }

            [JsonProperty("target_is_static")]
            public bool target_is_static { get; set; }

            [JsonProperty("chirality")]
            public string chirality { get; set; }

            [JsonProperty("target_celest")]
            public Celest target_celest { get; set; }

            [JsonProperty("target_altaz")]
            public Altaz target_altaz { get; set; }

            [JsonProperty("target_home")]
            public bool target_home { get; set; }
        }
    }


    class TelescopeApiClient : ApiClient<DefaultReadyRepr>
    {
        public TelescopeApiClient(string Proto, string Server, int Port) : base(Proto, Server, Port)
        { }

        public TelescopeApiClient(string Proto, string Server, int Port, TraceLogger tracelogger) : base(Proto, Server, Port)
        {
            tl = tracelogger;
        }

        public StateRepr state
        {
            get
            {
                refreshStaleState();
                return _state;
            }
        }

        protected override string _DevPath()
        {
            return "telescope";
        }

        protected StateRepr _state;
        protected DateTime? stateLastRefreshed = null;
        
        protected void refreshStaleState()
        {
            DateTime aSecAgo = DateTime.Now.Subtract(new TimeSpan(0,0,1));
            if (stateLastRefreshed == null || stateLastRefreshed < aSecAgo)
            {
                stateLastRefreshed = DateTime.Now;
                //_state = ExecuteGET<StateRepr>("state");
            }
            _state = ExecuteGET<StateRepr>("state");
        }

        public bool GetSlewing()
        {
            refreshStaleState();
            //bool slewing = _state.current.slewing;
            LogMessage(
                "GetTracking",
                "ready: " + _state.ready.ToString() +
                ", config.tracking: " + _state.config.tracking.ToString() +
                ", current.on_target: " + _state.current.on_target.ToString());
            bool slewing = _state.ready && _state.config.tracking && !_state.current.on_target;
            return slewing;
        }

        public bool GetTracking()
        {
            refreshStaleState();
            LogMessage(
                "GetTracking",
                "ready: " + _state.ready.ToString() + 
                ", config.tracking: " + _state.config.tracking.ToString() +
                ", config.target_is_static: " + _state.config.target_is_static.ToString() + 
                ", current.on_target: " + _state.current.on_target.ToString());
            return _state.ready && _state.config.tracking && !_state.config.target_is_static && _state.current.on_target;
        }

        public void SetTracking(bool value)
        {
            refreshStaleState();
            _state.config.target_is_static = !value;
            _state.config.tracking = value;
            _state.config.target_celest.ra = _state.current.celest.ra;
            _state.config.target_celest.de = _state.current.celest.de;
            _state.config.target_altaz = null;
            _state = ExecutePUT<StateRepr, StateRepr>("state", _state);
            refreshStaleState();
        }

        public void SetTargetCelest(double ra, double de)
        {
            refreshStaleState();
            _state.config.target_celest.ra = ra;
            _state.config.target_celest.de = de;
            _state.config.target_altaz = null;
            _state = ExecutePUT<StateRepr, StateRepr>("state", _state);
            refreshStaleState();
        }

        public void SetTargetAltaz(double alt, double az)
        {
            refreshStaleState();
            _state.config.target_altaz.alt = alt;
            _state.config.target_altaz.az = az;
            _state.config.target_home = false;
            _state = ExecutePUT<StateRepr, StateRepr>("state", _state);
            refreshStaleState();
        }

        public void SetCelest(double ra, double de)
        {
            refreshStaleState();
            _state.current.celest.ra = ra;
            _state.current.celest.de = de;
            _state = ExecutePOST<StateRepr, StateRepr>("state", _state);
            refreshStaleState();
        }

        public bool GetAtPark()
        {
            refreshStaleState();
            bool atHome = _state.current.at_home;
            LogMessage(
                "GetAtPark", atHome.ToString());
            return atHome;
        }

        public void SetTargetPark()
        {
            _state.config.target_home = true;
            _state = ExecutePUT<StateRepr, StateRepr>("state", _state);
            refreshStaleState();
        }

        internal static TraceLogger tl;
        internal static void LogMessage(string identifier, string message, params object[] args)
        {
            var msg = string.Format(message, args);
            tl.LogMessage(identifier, msg);
        }
        
        protected ResponseRepr ExecutePOST<ResponseRepr, RequestRepr>(string url, RequestRepr request_repr)
        {
            string content = JsonConvert.SerializeObject(request_repr);
            int retries = Retries;
            if (!string.IsNullOrEmpty(url) && !url.EndsWith("/"))
                url += "/";
            while (retries-- > 0)
            {
                StringContent request_content = new StringContent(content, Encoding.UTF8, "application/json");
                HttpResponseMessage response = client.PostAsync(url, request_content).Result;

                if (response.IsSuccessStatusCode)
                {
                    // Parse the response body. Blocking!
                    var response_content = response.Content.ReadAsStringAsync().Result;
                    var response_repr = JsonConvert.DeserializeObject<ResponseRepr>(response_content);
                    return response_repr;
                }
                LogMessage("ExecutePOST", "Fail - sleep 300");
                System.Threading.Thread.Sleep(300);
            }
            throw new CommError("Retries exhausted");
        }
    }
}
