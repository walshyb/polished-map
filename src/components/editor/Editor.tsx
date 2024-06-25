import Metatiles from "./Metatiles";
import Map from "./map/Map";

interface EditorProps {
  metatiles: any;
}

export default function Editor({ metatiles }: EditorProps) {
  return (
    <div>
      <Metatiles metatiles={metatiles} />
      <Map />
    </div>
  );
}
