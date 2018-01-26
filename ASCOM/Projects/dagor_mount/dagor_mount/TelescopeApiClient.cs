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

        [JsonProperty("current")]
        public Current current { get; set; }
        public class Current
        {

            [JsonProperty("chirality")]
            public string chirality { get; }

            [JsonProperty("celest")]
            public Celest celest { get; set; }
            public class Celest
            {
                [JsonProperty("ra")]
                public double ra { get; set; }

                [JsonProperty("de")]
                public double de { get; set; }
            }

        }

        [JsonProperty("config")]
        public Config config { get; set; }
        public class Config
        {
            [JsonProperty("tracking")]
            public bool tracking { get; set; }

            [JsonProperty("chirality")]
            public string chirality { get; }

            [JsonProperty("target_celest")]
            public TargetCelest target_celest { get; set; }
            public class TargetCelest
            {
                [JsonProperty("ra")]
                public double ra { get; set; }

                [JsonProperty("de")]
                public double de { get; set; }
            }
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
                _state = ExecuteGET<StateRepr>("state");
            }
        }
        
    }
}
