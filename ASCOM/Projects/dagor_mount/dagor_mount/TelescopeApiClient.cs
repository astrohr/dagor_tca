using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ASCOM.DagorApiClient;
using Newtonsoft.Json;

namespace ASCOM.DagorTelescope
{

    public class StateRepr : IRepr
    {


        public class Celest
        {
            [JsonProperty("ra")]
            public double ra { get; set; }

            [JsonProperty("de")]
            public double de { get; set; }
        }


        [JsonProperty("ready")]
        public bool ready { get; }

        [JsonProperty("current")]
        public Current current { get; set; }
        public class Current
        {

            [JsonProperty("chirality")]
            public string chirality { get; }

            [JsonProperty("slewing")]
            public bool slewing { get; }

            [JsonProperty("on_target")]
            public bool on_target { get; }

            [JsonProperty("celest")]
            public Celest celest { get; set; }
           

            [JsonProperty("altaz")]
            public Altaz altaz { get; set; }
            public class Altaz
            {
                [JsonProperty("alt")]
                public double alt { get; set; }

                [JsonProperty("az")]
                public double az { get; set; }
            }

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
            public string chirality { get; }

            [JsonProperty("target_celest")]
            public Celest target_celest { get; set; }
        }
    }


    class TelescopeApiClient : ApiClient<DefaultReadyRepr>
    {
        public TelescopeApiClient(string Proto, string Server, int Port) : base(Proto, Server, Port)
        { }

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

        public bool GetTracking()
        {
            refreshStaleState();
            _state = ExecuteGET<StateRepr>("state");
            return _state.config.tracking && ! _state.current.slewing && _state.config.target_is_static;
        }

        public void SetTracking(bool value)
        {
            refreshStaleState();
            _state.config.target_is_static = !value;
            if (! value)
            {
                _state.config.target_celest = _state.current.celest;
            }
            _state = ExecutePUT<StateRepr, StateRepr>("state", _state);
        }

        public void SetTargetCelest(double ra, double de)
        {
            refreshStaleState();
            _state.config.target_celest.ra = ra;
            _state.config.target_celest.de = de;
            _state = ExecutePUT<StateRepr, StateRepr>("state", _state);
        }

    }
}
